//============================================================================
//
//% Student Name 1: student1
//% Student 1 #: 123456781
//% Student 1 userid (email): stu1 (stu1@sfu.ca)
//
//% Student Name 2: student2
//% Student 2 #: 123456782
//% Student 2 userid (email): stu2 (stu2@sfu.ca)
//
//% Below, edit to list any people who helped you with the code in this file,
//%      or put ‘none’ if nobody helped (the two of) you.
//
// Helpers: _everybody helped us/me with the assignment (list names or put ‘none’)__
//
// Also, list any resources beyond the course textbook and the course pages on Piazza
// that you used in making your submission.
//
// Resources:  ___________
//
//%% Instructions:
//% * Put your name(s), student number(s), userid(s) in the above section.
//% * Enter the above information in Test.cpp if you are submitting that file too.
//% * Edit the "Helpers" line and "Resources" line.
//% * Form groups as described at:  https://courses.cs.sfu.ca/docs/students
//% * Your group name should be "P3_<userid1>_<userid2>" (eg. P3_stu1_stu2)
//% * Submit one or two files to courses.cs.sfu.ca
//
// Name        : TreeNode.hpp
// Description : Course Project Part 3 (Directory Tree)
//============================================================================

#ifndef SRC_TREENODE_HPP_
#define SRC_TREENODE_HPP_

//#include <iostream>
#include <vector>

namespace ensc251 {
	// using namespace std;

	class TreeNode;

	typedef void (TreeNode::*action_func)(void) const;

	class TreeNode
	{
		typedef std::vector<TreeNode*> TreeNodePVect;

		protected:
			TreeNodePVect childPVector;

		public:
			TreeNode() {};
			TreeNode(const TreeNode& nodeToCopy) { 
				for (auto childP: nodeToCopy.childPVector) {
					add_childP(childP->clone());
				}
			}

			virtual TreeNode* clone() const { return new TreeNode(*this); };

			virtual ~TreeNode() {
				OUT << "Destroying TreeNode with " << childPVector.size() << " children."<< std::endl;
				for (auto child : childPVector)
					delete child;
			}

		    void swap(TreeNode & other) // the swap member function (should never fail!)
		    {
		        std::swap(childPVector, other.childPVector);
		    }

		    TreeNode & operator = (TreeNode other) // note: argument passed by value, copy made!
		    {
		        swap(other); // swap *this with other
		        return *this; // by convention, always return *this
		    }// swapped other (original *this) is destroyed, releasing the memory

			const TreeNodePVect& get_children() const { return childPVector; }
			void add_childP(TreeNode* child){ childPVector.push_back(child); }
			void add_children(const TreeNodePVect& childPV){ childPVector.insert(childPVector.end(), childPV.begin(), childPV.end()); }

			virtual void traverse_node(action_func f) const
			{
				if(childPVector.size() > 2)
					post_order_traverse_node(f);
				else if(childPVector.size() == 2)
					in_order_traverse_node(f);
				else
					pre_order_traverse_node(f);
			}

			void in_order_traverse_node(action_func f) const
			{
				if(childPVector.size() != 2)
				{
					OUT << "Error -- can only do inorder traversal on a node with 2 children" << std::endl;
					return;
				}
				childPVector[0]->traverse_node(f);
				(this->*f)();
				childPVector[1]->traverse_node(f);
			}

			void pre_order_traverse_node(action_func f) const
			{
				(this->*f)();
				for(auto child : childPVector)
				{
					child->traverse_node(f);
				}
			}

			void post_order_traverse_node(action_func f) const
			{
				for(auto childP : childPVector)
				{
					childP->traverse_node(f);
				}
				(this->*f)();
			}
	};

}

#endif /* SRC_TREENODE_HPP_ */
