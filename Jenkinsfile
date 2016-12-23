#!groovy

// Jenkinsfile for compiling, testing, and packaging Aseba.
// Requires CMake plugin from https://github.com/davidjsherman/aseba-jenkins.git in global library.

// Ideally we should unarchive previously compiled Dashel and Enki libraries, but for
// now we conservatively recompile them.

pipeline {
	agent label:'' // use any available Jenkins agent

	// Jenkins will prompt for parameters when a branch is build manually
	// but will use default parameters when the entire project is built.
	parameters {
		stringParam(defaultValue: 'master', description: 'Dashel branch', name: 'branch_dashel')
		stringParam(defaultValue: 'master', description: 'Enki branch', name: 'branch_enki')
	}
	
	stages {
		stage('Prepare') {
			steps {
				echo "branch_dashel=${env.branch_dashel}"
				echo "branch_enki=${env.branch_enki}"

				// Dashel and Enki will be checked ou into externals/ directory.
				// Everything will be built in build/ directory.
				// Everything will be installed in dist/ directory.
				sh 'mkdir -p externals build dist'
				dir('aseba') {
					checkout scm
					sh 'git submodule update --init'
				}
				dir('externals/dashel') {
					git branch: "${env.branch_dashel}", url: 'https://github.com/aseba-community/dashel.git'
				}
				dir('externals/enki') {
					git branch: "${env.branch_enki}", url: 'https://github.com/enki-community/enki.git'
				}
				stash excludes: '.git', name: 'source'
			}
		}
		stage('Dashel') {
			steps {
				parallel (
					"debian" : {
						node('debian') {
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/dashel', label: 'debian',
								   buildDir: '$workDir/build/dashel/debian'])
							stash includes: 'dist/**', name: 'dist-debian'
						}
					},
					"macos" : {
						node('macos') {
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/dashel', label: 'macos',
								   buildDir: '$workDir/build/dashel/macos'])
							stash includes: 'dist/**', name: 'dist-macos'
						}
					},
					"windows" : {
						node('windows') {
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/dashel', label: 'windows',
								   buildDir: '$workDir/build/dashel/windows'])
							stash includes: 'dist/**', name: 'dist-windows'
						}
					}
				)
			}
		}
		stage('Enki') {
			steps {
				parallel (
					"debian" : {
						node('debian') {
							unstash 'dist-debian'
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/enki', label: 'debian',
								   buildDir: '$workDir/build/enki/debian'])
							stash includes: 'dist/**', name: 'dist-debian'
						}
					},
					"macos" : {
						node('macos') {
							unstash 'dist-macos'
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/enki', label: 'macos',
								   buildDir: '$workDir/build/enki/macos'])
							stash includes: 'dist/**', name: 'dist-macos'
						}
					},
					"windows" : {
						node('windows') {
							unstash 'dist-windows'
							unstash 'source'
							CMake([sourceDir: '$workDir/externals/enki', label: 'windows',
								   buildDir: '$workDir/build/enki/windows'])
							stash includes: 'dist/**', name: 'dist-windows'
						}
					}
				)
			}
		}
		stage('Compile') {
			steps {
				parallel (
					"debian" : {
						node('debian') {
							unstash 'dist-debian'
							script {
								env.dashel_DIR = sh ( script: 'dirname $(find dist -name dashelConfig.cmake | head -1)', returnStdout: true).trim()
								env.enki_DIR = sh ( script: 'dirname $(find dist -name enkiConfig.cmake | head -1)', returnStdout: true).trim()
							}
							unstash 'source'
							CMake([sourceDir: '$workDir/aseba', label: 'debian',
								   envs: [ "dashel_DIR=${env.dashel_DIR}", "enki_DIR=${env.enki_DIR}" ] ])
							stash includes: 'dist/**', name: 'dist-debian'
						}
					},
					"macos" : {
						node('macos') {
							unstash 'dist-macos'
							script {
								env.dashel_DIR = sh ( script: 'dirname $(find dist -name dashelConfig.cmake | head -1)', returnStdout: true).trim()
								env.enki_DIR = sh ( script: 'dirname $(find dist -name enkiConfig.cmake | head -1)', returnStdout: true).trim()
							}
							unstash 'source'
							CMake([sourceDir: '$workDir/aseba', label: 'macos',
								   envs: [ "dashel_DIR=${env.dashel_DIR}", "enki_DIR=${env.enki_DIR}" ] ])
							stash includes: 'dist/**', name: 'dist-macos'
						}
					},
					"windows" : {
						node('windows') {
							unstash 'dist-windows'
							script {
								env.dashel_DIR = sh ( script: 'dirname $(find dist -name dashelConfig.cmake | head -1)', returnStdout: true).trim()
								env.enki_DIR = sh ( script: 'dirname $(find dist -name enkiConfig.cmake | head -1)', returnStdout: true).trim()
							}
							unstash 'source'
							CMake([sourceDir: '$workDir/aseba', label: 'windows',
								   envs: [ "dashel_DIR=${env.dashel_DIR}", "enki_DIR=${env.enki_DIR}" ] ])
							stash includes: 'dist/**', name: 'dist-windows'
						}
					}
				)
			}
		}
		stage('Test') {
			// Only do some tests. To do: add test labels in CMake to distinguish between
			// obligatory smoke tests (to be done for every PR) and extended tests only for
			// releases or for end-to-end testing.
			steps {
				node('debian') {
					unstash 'dist-debian'
					dir('build/aseba') {
						sh "LANG=C ctest -E 'e2e.*|simulate.*|.*http.*|valgrind.*'"
					}
				}
			}
		}
		stage('Extended Test') {
			// Extended tests are only run for master branch.
			when {
				env.BRANCH == 'master'
			}
			steps {
				node('debian') {
					unstash 'dist-debian'
					dir('build/aseba') {
						sh "LANG=C ctest -R 'e2e.*|simulate.*|.*http.*|valgrind.*'"
					}
				}
			}
		}
		stage('Package') {
			// Only do packaging if boolean parameter says so.
			// For now, this pipeline only knows about making .deb on Debian.
			when {
				env.BRANCH == 'master'
			}
			steps {
				parallel (
					"debian" : {
						node('debian') {
							unstash 'dist-debian'
							unstash 'source'
							dir('dashel') {
								sh 'which debuild && debuild -i -us -uc -b'
							}
							sh 'mv aseba*.deb aseba*.changes aseba*.build dist/debian/'
							stash includes: 'dist/**', name: 'dist-debian'
						}
					}
				)
			}
		}
		stage('Archive') {
			steps {
				script {
					// Can't use collectEntries yet [JENKINS-26481]
					def p = [:]
					for (x in ['debian','macos','windows']) {
						def label = x
						p[label] = {
							node(label) {
								unstash 'dist-' + label
								archiveArtifacts artifacts: 'dist/**', fingerprint: true, onlyIfSuccessful: true
							}
						}
					}
					parallel p;
				}
			}
		}
	}
}
