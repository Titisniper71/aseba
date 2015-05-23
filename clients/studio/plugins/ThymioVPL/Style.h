#ifndef VPL_STYLE_H
#define VPL_STYLE_H

#include <QColor>

namespace Aseba { namespace ThymioVPL {
	
	//! Style metrics for VPL
	struct Style
	{
		static const int blockWidth;
		static const int blockHeight;
		static const int blockSpacing;
		static const int blockDropAreaBorderWidth;
		static const qreal blockDropAreaSaturationFactor;
		static const qreal blockDropAreaValueFactor;
		
		static const int addRemoveButtonWidth;
		static const int addRemoveButtonHeight;
		
		static const int removeBlockButtonWidth;
		static const int removeBlockButtonHeight;
		
		static const int eventActionsSetColumnWidth;
		static const int eventActionsSetRowStep;
		static const int eventActionsSetCornerSize;
		static const QColor eventActionsSetBackgroundColors[2];
		static const QColor eventActionsSetForegroundColors[2];
		
		static const int addRemoveButtonCornerSize;
		static const QColor addRemoveButtonBackgroundColor;
		
		static QColor blockCurrentColor(const QString& type);
		static QColor blockColor(const QString& type, unsigned index);
		static void blockSetCurrentColorIndex(unsigned index);
		static unsigned blockColorsCount();
		
		static QColor unusedButtonFillColor;
		static QColor unusedButtonStrokeColor;
	};
	
} }  // namespace ThymioVPL / namespace Aseba

#endif // VPL_STYLE_H

