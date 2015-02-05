/*****************************************************************************\
 *  grid.h - Functions to manage inter-cluster job IDs
 *****************************************************************************
 *  Copyright (C) 2015 Swiss National Supercomputing Centre
 *  Written by Stephen Trofinoff
 *
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _HAVE_SLURMCTLD_GRID_H
#define _HAVE_SLURMCTLD_GRID_H

#include <unistd.h>
#include <sys/types.h>

extern int slurmctld_grid_update_request(void *db_conn);

extern int request_sicp_job_id(void *db_conn, uint32_t *job_id);

extern int request_sicp_jobid_cluster_idx(void *db_conn, uint32_t job_id,
					  int *idx);

#endif /* !_HAVE_SLURMCTLD_GRID_H */
