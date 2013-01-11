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
#ifndef GitanicLayoutMutator_h
#define GitanicLayoutMutator_h



#include "C4Plugins.h"
#include "C4Base.h"
#include "C4Map.h"

#include "LayoutMutator.h"
#include "PanelMutatorTree.h"


extern "C"
{
    C4MODULEEXPORT C4::Plugin* ConstructPlugin();
};

namespace C4
{
	
	class LayoutMutator;
	class MutatorTreeMap;
	class GitanicLayoutMutatorPlugin : public Plugin, public Singleton<GitanicLayoutMutatorPlugin>
	{
		private:
			MutatorReg<LayoutMutator> layoutMutatorReg;
			MutatorTreeMap * rootPanelMap;
			CommandObserver<GitanicLayoutMutatorPlugin> openPanelObserver;
		public:
			
			void OpenPanelFunction(Command *command, const char *text);
			MutatorTreeMap * getMutatorTreeMap(void){
				return rootPanelMap;
			}
			GitanicLayoutMutatorPlugin();
			~GitanicLayoutMutatorPlugin();	
	};
	extern GitanicLayoutMutatorPlugin * TheGitanicLayoutMutatorPlugin;
}


#endif

// ZYVRVOM
