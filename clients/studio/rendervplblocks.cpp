/*
	Aseba - an event-based framework for distributed robot control
	Copyright (C) 2007--2015:
		Stephane Magnenat <stephane at magnenat dot net>
		(http://stephane.magnenat.net)
		and other contributors, see authors.txt for details
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation, version 3 of the License.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include <signal.h>
#include <QApplication>
#include <QString>
#include <QSvgGenerator>
#include <QPainter>
#include <QSize>
#include <QDebug>
#include <iostream>
#include <stdexcept>
#include "plugins/ThymioVPL/EventBlocks.h"
#include "plugins/ThymioVPL/ActionBlocks.h"
#include "plugins/ThymioVPL/StateBlocks.h"

using namespace std;

/**
	\defgroup rendervplblocks Helper application that generates SVG files of VPL blocks
*/

namespace Aseba { namespace ThymioVPL {
	
	void setupGenerator(QSvgGenerator& generator, QString& templateName)
	{
		generator.setFileName(templateName + ".svg");
		generator.setSize(QSize(256, 256));
		generator.setViewBox(QRectF(0, 0, 256, 256));
		generator.setTitle("VPL " + templateName);
		generator.setDescription("This image was generated by an helper tool part of Aseba, get it at http://thymio.org");
		
	}
	
	template<typename T>
	void renderBlock(QString templateName)
	{
		QSvgGenerator generator;
		setupGenerator(generator, templateName);
		
		QPainter painter(&generator);
		
		T block;
		block.render(painter);
	}
	
	template<typename T>
	void renderBlock(QString templateName, bool advanced)
	{
		QSvgGenerator generator;
		setupGenerator(generator, templateName);
		
		QPainter painter(&generator);
		
		T block(advanced);
		block.render(painter);
	}
	
	template<typename T>
	void renderBlockValue(QString templateName, unsigned i, unsigned value)
	{
		QSvgGenerator generator;
		setupGenerator(generator, templateName);
		
		QPainter painter(&generator);
		
		T block(true);
		block.setValue(i,value);
		block.render(painter);
	}
	
	void renderBlocks()
	{
		// events
		renderBlock<ArrowButtonsEventBlock>("event-buttons");
		renderBlock<ProxEventBlock>("event-prox", false);
		renderBlock<ProxEventBlock>("event-prox-advanced", true);
		renderBlock<ProxGroundEventBlock>("event-prox-ground", false);
		renderBlock<ProxGroundEventBlock>("event-prox-ground-advanced", true);
		renderBlock<AccEventBlock>("event-tap", false);
		renderBlock<AccEventBlock>("event-tap-advanced", true);
		renderBlockValue<AccEventBlock>("event-roll", 0, 1);
		renderBlockValue<AccEventBlock>("event-pitch", 0, 2);
		renderBlock<ClapEventBlock>("event-clap");
		renderBlock<TimeoutEventBlock>("");
		
		// filters
		renderBlock<StateFilterCheckBlock>("state-filter");
		
		// actions
		renderBlock<MoveActionBlock>("action-motors");
		renderBlock<TopColorActionBlock>("action-colors-up");
		renderBlock<BottomColorActionBlock>("action-colors-down");
		renderBlock<SoundActionBlock>("action-music");
		renderBlock<TimerActionBlock>("action-timer");
		renderBlock<StateFilterActionBlock>("action-states");
	}
	
} } // namespace ThymioVPL / namespace Aseba

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	Aseba::ThymioVPL::renderBlocks();
	
	return 0;
}


