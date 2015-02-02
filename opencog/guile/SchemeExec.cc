/*
 * SchemeExec.cc
 *
 * Execute ExecutionOutputLink's
 * Copyright (c) 2009 Linas Vepstas <linasvepstas@gmail.com>
 */

#ifdef HAVE_GUILE

#include <cstddef>
#include <libguile.h>
#include <opencog/execution/ExecutionOutputLink.h>

#include "SchemeEval.h"
#include "SchemeSmob.h"

using namespace opencog;

/**
 * do_apply -- apply named function func to arguments in ListLink
 * It is assumed that varargs is a ListLink, containing a list of
 * atom handles. This list is unpacked, and then the fuction func
 * is applied to them. If the function returns an atom handle, then
 * this is returned.
 */
Handle SchemeEval::do_apply(const std::string &func, Handle& varargs)
{
	// Apply the function to the args
	SCM sresult = do_apply_scm (func, varargs);
	
	// If the result is a handle, return the handle.
	return SchemeSmob::scm_to_handle(sresult);
}

/**
 * do_apply_scm -- apply named function func to arguments in ListLink
 * It is assumed that varargs is a ListLink, containing a list of
 * atom handles. This list is unpacked, and then the fuction func
 * is applied to them. The SCM value returned by the function is returned.
 */
SCM SchemeEval::do_apply_scm(const std::string& func, Handle& varargs )
{
	SCM sfunc = scm_from_utf8_symbol(func.c_str());
	SCM expr = SCM_EOL;
	
	// If there were args, pass the args to the function.
	const std::vector<Handle> &oset = atomspace->getOutgoing(varargs);
		
	size_t sz = oset.size();
	for (int i=sz-1; i>=0; i--)
	{
		Handle h = oset[i];
		SCM sh = SchemeSmob::handle_to_scm(h);
		expr = scm_cons(sh, expr);
	}
	expr = scm_cons(sfunc, expr);
	return do_scm_eval(expr);
}

extern void tenter(void);
extern void tleave(void);
extern void reinit(void);
extern bool doit;
/**
 * Executes an ExecutionOutputLink
 */
SCM SchemeSmob::ss_execute (SCM satom)
{
reinit();
// tenter();
if (doit) { doit = false; tleave(); }

	AtomSpace* atomspace = ss_get_env_as("cog-execute");
    
	Handle h = verify_handle(satom, "cog-execute");
	
	if (h->getType() != EXECUTION_OUTPUT_LINK)
	{
		scm_wrong_type_arg_msg("cog-execute", 1, satom,
			"ExecutionOutputLink opencog cog-execute");
	}

	SCM rc =  handle_to_scm(ExecutionOutputLink::do_execute(atomspace, h));
// tleave();
	return rc;
}

#endif
/* ===================== END OF FILE ============================ */
