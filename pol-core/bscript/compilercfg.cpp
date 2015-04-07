/*
History
=======

Notes
=======

*/

#include "compilercfg.h"
#include "../plib/systemstate.h"

#include "../clib/cfgfile.h"
#include "../clib/cfgelem.h"
#include "../clib/dirlist.h"
#include "../clib/xmain.h"

#ifdef _MSC_VER
#pragma warning(disable:4996) // POSIX deprecation warnings for stricmp, getenv
#endif

namespace Pol {
  namespace Bscript {
	void CompilerConfig::Read( const std::string& path )
	{
	  Clib::ConfigFile cf( path.c_str() );
	  Clib::ConfigElem elem;
	  cf.readraw( elem );

	  PackageRoot.clear();
	  IncludeDirectory.clear();

      std::string tmp;
	  while ( elem.remove_prop( "PackageRoot", &tmp ) )
	  {
		PackageRoot.push_back( Clib::normalized_dir_form( tmp ) );
	  }
	  if ( elem.remove_prop( "IncludeDirectory", &tmp ) )
	  {
		IncludeDirectory = Clib::normalized_dir_form( tmp );
	  }
	  ModuleDirectory = Clib::normalized_dir_form( elem.remove_string( "ModuleDirectory" ) );
	  PolScriptRoot = Clib::normalized_dir_form( elem.remove_string( "PolScriptRoot" ) );
	  GenerateListing = elem.remove_bool( "GenerateListing", false );
	  GenerateDebugInfo = elem.remove_bool( "GenerateDebugInfo", false );
	  GenerateDebugTextInfo = elem.remove_bool( "GenerateDebugTextInfo", false );
	  DisplayWarnings = elem.remove_bool( "DisplayWarnings", false );
	  CompileAspPages = elem.remove_bool( "CompileAspPages", false );
	  AutoCompileByDefault = elem.remove_bool( "AutoCompileByDefault", false );
	  UpdateOnlyOnAutoCompile = elem.remove_bool( "UpdateOnlyOnAutoCompile", false );
	  OnlyCompileUpdatedScripts = elem.remove_bool( "OnlyCompileUpdatedScripts", false );
	  DisplaySummary = elem.remove_bool( "DisplaySummary", false );
	  OptimizeObjectMembers = elem.remove_bool( "OptimizeObjectMembers", true );
	  ErrorOnWarning = elem.remove_bool( "ErrorOnWarning", false );
	  GenerateDependencyInfo = elem.remove_bool( "GenerateDependencyInfo", OnlyCompileUpdatedScripts );

	  DisplayUpToDateScripts = elem.remove_bool( "DisplayUpToDateScripts", true );
	  ThreadedCompilation = elem.remove_bool( "ThreadedCompilation", false );
	  ParanoiaWarnings = elem.remove_bool( "ParanoiaWarnings", false );

	  // This is where we TRY to validate full paths from what was provided in the
	  // ecompile.cfg. Maybe Turley or Shini can find the best way to do this in *nix.
#ifdef WIN32
      std::string MyPath = path.c_str();
	  // If it's just "ecompile.cfg", let's change it to the exe's path which it SHOULD be
	  // with. 
	  if ( stricmp( MyPath.c_str(), "ecompile.cfg" ) == 0 )
	  {
		  std::string workingDir = Pol::Plib::systemstate.getWorkingDirectory();

		// Let's find the NEXT-TO-LAST / in the path, and remove from there on. Oh yay!
		// To bad we can't just force everyone to use ABSOLUTE PATHS NANDO. :o
		MyPath = workingDir.substr( 0, workingDir.length() - 1 );
		MyPath = MyPath.substr( 0, MyPath.find_last_of( '/' ) + 1 );
	  }
      if (IncludeDirectory.find(':') == std::string::npos)
	  {
		if ( IncludeDirectory.substr( 0, 1 ) != "." ) // Let's make sure they didn't try using this method
		{
		  IncludeDirectory = MyPath + IncludeDirectory;
		}
	  }
      if (ModuleDirectory.find(':') == std::string::npos)
	  {
		if ( ModuleDirectory.substr( 0, 1 ) != "." ) // Let's make sure they didn't try using this method
		{
		  ModuleDirectory = MyPath + ModuleDirectory;
		}
	  }
      if (PolScriptRoot.find(':') == std::string::npos)
	  {
		if ( PolScriptRoot.substr( 0, 1 ) != "." ) // Let's make sure they didn't try using this method
		{
		  PolScriptRoot = MyPath + PolScriptRoot;
		}
	  }
	  for ( unsigned pr = 0; pr < PackageRoot.size(); ++pr )
	  {
          if (PackageRoot[pr].find(':') == std::string::npos)
		{
		  if ( PackageRoot[pr].substr( 0, 1 ) != "." ) // Let's make sure they didn't try using this method
		  {
			PackageRoot[pr] = MyPath + PackageRoot[pr];
		  }
		}
	  }

#endif
	}

	void CompilerConfig::SetDefaults()
	{
	  const char* tmp;

	  tmp = getenv( "ECOMPILE_PATH_EM" );
	  ModuleDirectory = tmp ? Clib::normalized_dir_form( tmp ) : Pol::Plib::systemstate.getWorkingDirectory();

	  tmp = getenv( "ECOMPILE_PATH_INC" );
	  IncludeDirectory = tmp ? Clib::normalized_dir_form( tmp ) : Pol::Plib::systemstate.getWorkingDirectory();

	  PolScriptRoot = IncludeDirectory;

	  DisplayUpToDateScripts = true;
	}

	CompilerConfig compilercfg;
  }
}
