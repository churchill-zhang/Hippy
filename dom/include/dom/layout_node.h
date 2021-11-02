#pragma once

#include "engine/HPNode.h"
#include <cstdint>

namespace hippy {
inline namespace dom {

class LayoutNode {
public:
  LayoutNode(HPNodeRef hp_node):hp_node_(hp_node)){};
  ~LayoutNode() { deallocate(); };
  void SetDirection(HPDirection direction);
  void SetWidth(float width);
  void SetHeight(float height);
  void SetMeasureFunction(HPMeasureFunc measure);

  void SetFlex(float flex);
  void SetFlexGrow(float flex_grow);
  void SetFlexShrink(float flex_shrink);
  void SetFlexBasis(float flex_basis);
  void SetFlexDirection(FlexDirection flex_direction);
  void SetPositionType(PositionType position_type);
  void SetPosition(CSSDirection css_direction, float position);
  void SetMargin(CSSDirection css_direction, float margin);
  void SetMarginAuto(CSSDirection css_direction);
  void SetPadding(CSSDirection css_direction, float padding);
  void SetBorder(CSSDirection css_direction, float border);
  void SetFlexWrap(FlexWrapMode wrap_mode);
  void SetJustifyContent(FlexAlign justify);
  void SetAlignContent(FlexAlign align_content);
  void SetAlignItems(FlexAlign align_items);
  void SetAlignSelf(FlexAlign align_self);
  void SetDisplay(DisplayType display_type);
  void SetMaxWidth(float max_width);
  void SetMaxHeight(float max_height);
  void SetMinWidth(float min_width);
  void SetMinHeight(float min_height);
  void SetNodeType(NodeType node_type);
  void SetOverflow(OverflowType over_flow_type);

  float GetLeft();
  float GetTop();
  float GetRight();
  float GetBottom();
  float GetWidth();
  float GetHeight();
  float GetMargin();
  float GetPadding();
  float GetBorder();
  bool GetHadOverflow();

  bool InsertChild(const shared_ptr<LayoutNode> child, uint32_t index);
  bool RemoveChild(const shared_ptr<LayoutNode> child);

  bool HasNewLayout();
  bool SetHasNewLayout(bool has_new_layout);
  void MarkDirt();
  bool IsDirty();

  void DoLayout(float parent_width, float parent_height,
                HPDirection direction = DirectionLTR,
                void *layout_context = nullptr);
  void Print();
  bool Reset();

  void deallocate();

private:
  std::shared_ptr<LayoutNode> parent_;
  std::vector<LayoutNode> children_;
  int32_t index;

  HPNodeRef hp_node_;

  float width_ = 0;
  float height_ = 0;
  float top_ = 0;
  float left_ = 0;
  float margin_left_ = 0;
  float margin_top_ = 0;
  float margin_right_ = 0;
  float margin_bottom_ = 0;
  float padding_left_ = 0;
  float padding_top_ = 0;
  float padding_right_ = 0;
  float padding_bottom_ = 0;
  float border_left_ = 0;
  float border_top_ = 0;
  float border_right_ = 0;
  float border_bottom_ = 0;
  uint32_t border_color_ = 0;
  int32_t edge_set_flag_ = 0;
};

} // namespace dom
} // namespace hippy
