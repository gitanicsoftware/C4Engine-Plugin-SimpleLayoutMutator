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

#include "LayoutMutator.h"
#include "C4Configuration.h"
#include "C4String.h"


using namespace C4;


LayoutMutator::LayoutMutator() : Mutator(kMutatorLayout){
	widthPercentage = 0;
	widthPixels = 0;
	

	alignToWidget = nullptr;

	alignToTargetType = kAlignToTargetTypePanel;
	targetAlignLocation = kAlignTopLeft;
	widgetAlignLocation = kAlignTopLeft;
	
	xOffsetType = kSettingLayoutSizePixel;
	yOffsetType = kSettingLayoutSizePixel;
	xOffset = 0;
	yOffset = 0;

	widthType = kSettingLayoutSizePixel;
	heightType = kSettingLayoutSizePixel;
	width = 0;
	height = 0;

	findWidget = false;

	for(int i = 0; i < kSideCount; i++){
		margin[i] = 0;
		marginType[i] = kSettingLayoutSizePixel;
		offset[i] = 0; 
		offsetType[i] = kSettingLayoutSizePixel;
	}
	
	previousSize.x = 0;
	previousSize.y = 0;
	previousPosition.x = 0;
	previousPosition.y = 0;
	previousPosition.z = 0;

	dirty = true;
	resizeCheckOption = kRefreshContinually;

	Widget * rootPanel = nullptr;

}

void LayoutMutator::Preprocess(void){
	Mutator::Preprocess();
	if(width == 0){
		width = GetTargetWidget()->GetWidgetSize().x;
		height = GetTargetWidget()->GetWidgetSize().y;
	}
}

int32 LayoutMutator::GetSettingCount(void) const{
	return kSettingLayoutCount;
}

Setting  *LayoutMutator::GetSetting(int32 index) const{
	HeadingSetting * headingSetting;
	TextSetting * textSetting;
	MenuSetting * menuSetting;

	
	String<17> sizeString = String<17>("");

	switch(index){
	case kSettingLayoutSizeHeading:
		headingSetting = new HeadingSetting(kSettingLayoutSizeHeading,"Size");
		return headingSetting;

	case kSettingLayoutWidth:
		textSetting = new TextSetting(kSettingLayoutWidth,SizeToString(sizeString,widthType,width),"Width",16);
		return textSetting;

	case kSettingLayoutHeight:
		textSetting = new TextSetting(kSettingLayoutHeight,SizeToString(sizeString,heightType,height),"Height",16);
		return textSetting;

	case kSettingLayoutAlignmentHeading:
		headingSetting = new HeadingSetting(kSettingLayoutAlignmentHeading,"Alignment");
		return headingSetting;

	case kSettingLayoutAlignWidgetLocation:
		menuSetting = new MenuSetting(kSettingLayoutAlignWidgetLocation,widgetAlignLocation,"Align Widget's",kAlignLocationCount);
		menuSetting->SetMenuItemString(kAlignTopLeft,"Top Left");
		menuSetting->SetMenuItemString(kAlignTopCenter,"Top Center");
		menuSetting->SetMenuItemString(kAlignTopRight,"Top Right");
		menuSetting->SetMenuItemString(kAlignCenterLeft,"Center Left");
		menuSetting->SetMenuItemString(kAlignCenterCenter,"Center");
		menuSetting->SetMenuItemString(kAlignCenterRight,"Center Right");
		menuSetting->SetMenuItemString(kAlignBottomLeft,"Bottom Left");
		menuSetting->SetMenuItemString(kAlignBottomCenter,"Bottom Center");
		menuSetting->SetMenuItemString(kAlignBottomRight,"Bottom Right");
		return menuSetting;

	case kSettingLayoutAlignToTargetLocation:
		menuSetting = new MenuSetting(kSettingLayoutAlignToTargetLocation,targetAlignLocation,"Align To Target's",kAlignLocationCount);
		menuSetting->SetMenuItemString(kAlignTopLeft,"Top Left");
		menuSetting->SetMenuItemString(kAlignTopCenter,"Top Center");
		menuSetting->SetMenuItemString(kAlignTopRight,"Top Right");
		menuSetting->SetMenuItemString(kAlignCenterLeft,"Center Left");
		menuSetting->SetMenuItemString(kAlignCenterCenter,"Center");
		menuSetting->SetMenuItemString(kAlignCenterRight,"Center Right");
		menuSetting->SetMenuItemString(kAlignBottomLeft,"Bottom Left");
		menuSetting->SetMenuItemString(kAlignBottomCenter,"Bottom Center");
		menuSetting->SetMenuItemString(kAlignBottomRight,"Bottom Right");
		return menuSetting;
	 
	case kSettingLayoutAlignToWidgetName:
		textSetting = new TextSetting(kSettingLayoutAlignToWidgetName,"","Target Widget",16);
		if(alignToWidget && alignToWidget->GetWidgetKey().Length() > 0){
			textSetting->SetText(alignToWidget->GetWidgetKey());
		} else {
			textSetting->SetText("<panel>");
		}
		//alignToWidget = GetTargetWidget()->GetRootWidget()->FindWidget(setting->GetText());
		return textSetting;
	
	case kSettingLayoutBordersHeadering:
		headingSetting = new HeadingSetting(kSettingLayoutBordersHeadering,"Margin");
		return headingSetting;
	case kSettingLayoutMarginTop:
		textSetting = new TextSetting(kSettingLayoutMarginTop,SizeToString(sizeString,marginType[kSideTop],margin[kSideTop]),"Top",16);
		return textSetting;
	case kSettingLayoutMarginBottom:
		textSetting = new TextSetting(kSettingLayoutMarginBottom,SizeToString(sizeString,marginType[kSideBottom],margin[kSideBottom]),"Bottom",16);
		return textSetting;
	case kSettingLayoutMarginLeft:
		textSetting = new TextSetting(kSettingLayoutMarginLeft,SizeToString(sizeString,marginType[kSideLeft],margin[kSideLeft]),"Left",16);
		return textSetting;
	case kSettingLayoutMarginRight:
		textSetting = new TextSetting(kSettingLayoutMarginRight,SizeToString(sizeString,marginType[kSideRight],margin[kSideRight]),"Right",16);
		return textSetting;

	case kSettingLayoutOffsetHeadering:
		headingSetting = new HeadingSetting(kSettingLayoutOffsetHeadering,"Offset");
		return headingSetting;
	case kSettingLayoutOffsetTop:
		textSetting = new TextSetting(kSettingLayoutOffsetTop,SizeToString(sizeString,offsetType[kSideTop],offset[kSideTop]),"Top",16);
		return textSetting;
	case kSettingLayoutOffsetBottom:
		textSetting = new TextSetting(kSettingLayoutOffsetBottom,SizeToString(sizeString,offsetType[kSideBottom],offset[kSideBottom]),"Bottom",16);
		return textSetting;
	case kSettingLayoutOffsetLeft:
		textSetting = new TextSetting(kSettingLayoutOffsetLeft,SizeToString(sizeString,offsetType[kSideLeft],offset[kSideLeft]),"Left",16);
		return textSetting;
	case kSettingLayoutOffsetRight:
		textSetting = new TextSetting(kSettingLayoutOffsetRight,SizeToString(sizeString,offsetType[kSideRight],offset[kSideRight]),"Right",16);
		return textSetting;

	case kSettingLayoutOptimizationHeading:
		headingSetting = new HeadingSetting(kSettingLayoutOptimizationHeading,"Refresh");
		return headingSetting;
	case kSettingResizeCheckOption:
		menuSetting = new MenuSetting(kSettingResizeCheckOption,resizeCheckOption,"Resize Elements",kReseizeCheckOptionCount);
		menuSetting->SetMenuItemString(kRefreshContinually,"Continually");
		menuSetting->SetMenuItemString(kRefreshOnce,"Once");
		return menuSetting;
	}
	return new HeadingSetting(9999,"UNKNOWN SETTING");
}

/**
 * Formats an input string, and size type (kSettingLayoutSizePixel or 
 * kSettingLayoutSizePercent), treating the string as an integer and
 * then appending either 'px' or '%' depending on the type
 */
const char * LayoutMutator::SizeToString(String<17> &string, int32 sizeType, int32 size) const{
	string.Append(Text::IntegerToString(size),15);
		
	if(sizeType == kSettingLayoutSizePixel){
		string.Append("px",2);
	} else {
		string.Append("% ",2);
	}
	
	return (const char *)string;
}

/**
 * Parses the string representation of a size
 *
 * Size can be specified in XXXX% or XXXXpx. This method
 * reads an integer in from the input setting, then
 * assigns that value to the 'size' param. Based on the
 * presence (or absence) of a '%' sign, it also sets the
 * 'sizeType' to kSettingLayoutSizePercent or kSettingLayoutSizePixel 
 */
void LayoutMutator::ParseSizeInput(const TextSetting* setting, int32& sizeType, int32 & size){
	char curChar = 1;
	const char * settingText = setting->GetText();
	char integerAsString[16];
	Text::ReadInteger(settingText,integerAsString,9999);
	int tmpSize = Text::StringToInteger(integerAsString);
	
	size = tmpSize;
	
	if(Text::FindChar(settingText,'%') != -1){
		sizeType = kSettingLayoutSizePercent;
	} else {
		sizeType = kSettingLayoutSizePixel;
	}
}
void LayoutMutator::SetSetting(const Setting *setting){
	
	switch(setting->GetSettingIdentifier()){
	case kSettingLayoutHeight:
		ParseSizeInput(static_cast<const TextSetting*>(setting), heightType,height);
		break;
	case kSettingLayoutWidth:
		ParseSizeInput(static_cast<const TextSetting*>(setting), widthType,width);
		break;
	
	case kSettingLayoutAlignToTargetLocation:
		targetAlignLocation = static_cast<const MenuSetting*>(setting)->GetMenuSelection();
		break;
	case kSettingLayoutAlignWidgetLocation:
		widgetAlignLocation = static_cast<const MenuSetting*>(setting)->GetMenuSelection();
		break;
	case kSettingLayoutMarginTop:
		ParseSizeInput(static_cast<const TextSetting*>(setting), marginType[kSideTop],margin[kSideTop]);
		break;
	case kSettingLayoutMarginBottom:
		ParseSizeInput(static_cast<const TextSetting*>(setting), marginType[kSideBottom],margin[kSideBottom]);
		break;
	case kSettingLayoutMarginLeft:
		ParseSizeInput(static_cast<const TextSetting*>(setting),  marginType[kSideLeft],margin[kSideLeft]);
		break;
	case kSettingLayoutMarginRight:
		ParseSizeInput(static_cast<const TextSetting*>(setting),  marginType[kSideRight],margin[kSideRight]);
		break;
	case kSettingLayoutOffsetTop:
		ParseSizeInput(static_cast<const TextSetting*>(setting), offsetType[kSideTop],offset[kSideTop]);
		break;
	case kSettingLayoutOffsetBottom:
		ParseSizeInput(static_cast<const TextSetting*>(setting), offsetType[kSideBottom],offset[kSideBottom]);
		break;
	case kSettingLayoutOffsetLeft:
		ParseSizeInput(static_cast<const TextSetting*>(setting),  offsetType[kSideLeft],offset[kSideLeft]);
		break;
	case kSettingLayoutOffsetRight:
		ParseSizeInput(static_cast<const TextSetting*>(setting),  offsetType[kSideRight],offset[kSideRight]);
		break;
	case kSettingLayoutAlignToWidgetName:
		alignToWidget = GetTargetWidget()->GetRootWidget()->FindWidget(static_cast<const TextSetting*>(setting)->GetText());
		if(alignToWidget){
			alignToTargetType = kAlignToTargetTypeWidget;
		} else {
			alignToTargetType = kAlignToTargetTypePanel;
		}
		break;

	case kSettingResizeCheckOption:
		resizeCheckOption = static_cast<const MenuSetting*>(setting)->GetMenuSelection();
		break;
	}
	

}

LayoutMutator::LayoutMutator(const LayoutMutator& positionMutator) : Mutator(kMutatorLayout){
	widthPercentage = positionMutator.widthPercentage;
}
// Serialization functions
void LayoutMutator::Pack(Packer& data, unsigned_int32 packFlags) const{
	Mutator::Pack(data,packFlags);

	data << widgetAlignLocation;
	data << targetAlignLocation;
	data << alignToTargetType;
	data << widthType;
	data << heightType;
	data << width;
	data << height;

	data << resizeCheckOption;

	for(int i = 0; i < kSideCount; i++){
		data << marginType[i];
		data << margin[i];
	}
	for(int i = 0; i < kSideCount; i++){
		data << offsetType[i];
		data << offset[i];
	}

	if(alignToWidget){
		data << alignToWidget->GetWidgetKey();
	} else {
		data << String<kMaxWidgetKeyLength>();
	}
}
void LayoutMutator::Unpack(Unpacker& data, unsigned_int32 unpackFlags){
	Mutator::Unpack(data, unpackFlags);
	data >> widgetAlignLocation;
	data >> targetAlignLocation;
	data >> alignToTargetType;
	data >> widthType;
	data >> heightType;
	data >> width;
	data >> height;
	
	data >> resizeCheckOption;

	for(int i = 0; i < kSideCount; i++){
		data >> marginType[i];
		data >> margin[i];
	}
	for(int i = 0; i < kSideCount; i++){
		data >> offsetType[i];
		data >> offset[i];
	}

	data >> alignToWidgetKey;

	findWidget = true;
}

void LayoutMutator::Move(void){
	if(resizeCheckOption == kRefreshOnce && dirty == false){
		return;
	}

	if(findWidget){
		/* If the findWidget key has been set, that means we've just been unpacked, so
		find the widget, and clear the key*/
		if(alignToWidgetKey.Length() > 0){
			alignToWidget = GetTargetWidget()->GetRootWidget()->FindWidget(alignToWidgetKey);
			findWidget = false;
		}
	}
	Widget * widget = GetTargetWidget();
	
	Widget * rootPanel = widget->GetRootWidget();
	if(!rootPanel){
		return;
	}

	Point2D size = Point2D();
	Point3D position = Point3D();
	int32 parentX = 0;
	int32 parentY = 0;
	if(alignToTargetType == kAlignToTargetTypeWidget){
		if(alignToWidget){
			rootPanel = alignToWidget;
			parentX = alignToWidget->GetWidgetPosition().x;
			parentY = alignToWidget->GetWidgetPosition().y;
		} 
	}
	
	if(widthType == kSettingLayoutSizePercent){
		size.x = ((float) width / 100.0f) * rootPanel->GetWidgetSize().x;
	} else {
		size.x = width;
	}
	if(heightType == kSettingLayoutSizePercent){
		size.y = ((float) height / 100.0f) * rootPanel->GetWidgetSize().y;
	} else {
		size.y = height;
	}
	
	
	size.y -= margin[kSideTop] + margin[kSideBottom];
	size.x -= margin[kSideLeft] + margin[kSideRight];

	float targetAlignX[] = {0, -(size.x/2), - size.x};
	float targetAlignY[] = {0, -(size.y/2), -size.y};

	float a = rootPanel->GetWidgetPosition().x;
	float b = rootPanel->GetWidgetSize().x;

	float widgetAlignOffsetX[] = {0,(rootPanel->GetWidgetSize().x/2.0), rootPanel->GetWidgetSize().x};
	float widgetAlignOffsetY[] = {0,(rootPanel->GetWidgetSize().y/2.0), rootPanel->GetWidgetSize().y};
	widget->SetWidgetSize(size);
	float x = targetAlignX[widgetAlignLocation %3] +
			parentX +
			widgetAlignOffsetX[targetAlignLocation %3] + 
			margin[kSideLeft] + offset[kSideLeft] - offset[kSideRight];
	float y = targetAlignY[widgetAlignLocation/3] + 
			parentY + 
			widgetAlignOffsetY[targetAlignLocation /3] +
			margin[kSideTop] + offset[kSideTop] - offset[kSideBottom];
	position.x = x;
	position.y = y;
	position.z = widget->GetWidgetPosition().z;
	widget->SetWidgetPosition(
		position);
	widget->Invalidate();
	
	
	if(resizeCheckOption == kRefreshOnce && position == previousPosition && size == previousSize){
		dirty = false;
	}
	if(resizeCheckOption == kRefreshOnce){
		previousPosition = position;
		previousSize = size;
	}

	
}

Mutator *LayoutMutator::Replicate(void) const
{
	return (new LayoutMutator(*this));
}