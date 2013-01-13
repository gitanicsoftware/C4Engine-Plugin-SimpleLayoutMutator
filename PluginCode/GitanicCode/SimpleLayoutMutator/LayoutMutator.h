/*
Copyright (C) 2013 Chris Boyce <chris@gitanic.com>

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
THE SOFTWARE.
*/

#ifndef PositionedWidget_h
#define PositionedWidget_h

#include "C4Widgets.h"
#include "C4Interface.h"
#include "C4Configurable.h"
#include "C4Configuration.h"
#include "C4Types.h"

#include "LayoutMutatorPlugin.h"

namespace C4{
	enum{
		kMutatorLayout = 'LAYT'
	};
	enum {
		kSettingLayoutSizeHeading,
		kSettingLayoutWidth,
		kSettingLayoutHeight,

		kSettingLayoutAlignmentHeading,
		kSettingLayoutAlignWidgetLocation,
		kSettingLayoutAlignToTargetLocation,
		kSettingLayoutAlignToWidgetName,

		kSettingLayoutBordersHeadering,
		kSettingLayoutMarginTop,
		kSettingLayoutMarginBottom,
		kSettingLayoutMarginLeft,
		kSettingLayoutMarginRight,

		kSettingLayoutOffsetHeadering,
		kSettingLayoutOffsetTop,
		kSettingLayoutOffsetBottom,
		kSettingLayoutOffsetLeft,
		kSettingLayoutOffsetRight,

		kSettingLayoutOptimizationHeading,
		kSettingResizeCheckOption,

		kSettingLayoutCount
	};
	enum SizeType{
		kSettingLayoutSizePixel,
		kSettingLayoutSizePercent
	};
	enum AlignLocation{
		kAlignTopLeft,
		kAlignTopCenter,
		kAlignTopRight,
		kAlignCenterLeft,
		kAlignCenterCenter,
		kAlignCenterRight,
		kAlignBottomLeft,
		kAlignBottomCenter,
		kAlignBottomRight,

		kAlignLocationCount
	};
	enum AlignToTargetType{
		kAlignToTargetTypePanel,
		kAlignToTargetTypeWidget
	};

	enum Side{
		kSideTop,kSideBottom,kSideLeft,kSideRight,kSideCount};

	enum ResizeCheckOption{
		kRefreshContinually,
		kRefreshOnce,

		kReseizeCheckOptionCount
	};

	class LayoutMutator : public Mutator{
		friend class MutatorReg<LayoutMutator>;

	private:
		Point2D previousSize;
		Point3D previousPosition;

		int32 resizeCheckOption;
		float widthPercentage;
		float widthPixels;
		
		bool dirty;

		int32 widgetAlignLocation;
		int32 targetAlignLocation;

		int32 alignToTargetType;

		int32 xOffsetType;
		int32 yOffsetType;
		int32 xOffset;
		int32 yOffset;

		int32 widthType;
		int32 heightType;
		int32 width;
		int32 height;

		int32 marginType[4];
		int32 margin[4];

		int32 offsetType[4];
		int32 offset[4];
		
		int32 offsetTopType;
		bool findWidget;
		
		const char * SizeToString(String<17> &string, int32 sizeType, int32 size) const;
		Widget * alignToWidget;
		WidgetKey alignToWidgetKey;
		Mutator *Replicate(void) const override;
		LayoutMutator(const LayoutMutator& positionMutator);
		void ParseSizeInput(const TextSetting* setting, int32& sizeType, int32& size);
		
		
	public:
		void Preprocess(void);
		~LayoutMutator(){}
		LayoutMutator();
		void Move(void);
		int32 GetSettingCount(void) const;
		Setting *GetSetting(int32 index) const;
		void SetSetting(const Setting *setting);

		// Serialization functions
        void Pack(Packer& data, unsigned_int32 packFlags) const;
        void Unpack(Unpacker& data, unsigned_int32 unpackFlags);

	};
}
#endif