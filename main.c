// SPDX-License-Identifier: BSD-3-Clause
//
// inetutils Unikraft library
//
// Authors: Ethan Cotterell <e.cotterell@lancaster.ac.uk>
//
// Copyright (c) 2021, Lancaster University All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include <stdio.h>
#include <uk/config.h>

#ifdef CONFIG_INETUTILS_PROMETHEUS_SUPPORT
# include <uk/prom_glue.h>
#endif

int main(int argc, char *argv[])
{
	int ret;
    fprintf(stderr, "Starting main unikernel entrypoint\n");

#ifdef CONFIG_INETUTILS_PROMETHEUS_SUPPORT
	ret = uk_prom_init();
	if (ret) {
		fprintf(stderr, "Failed to initialize lib-prometheus. Exiting with code %d\n", ret);
		return ret;
	}
#endif
	
#ifdef CONFIG_LIBINETUTILS_PING_MAIN_FUNCTION
	ret = ping_main(argc, argv);
	if (ret) {
		fprintf(stderr, "ping main function failed. Exiting with code %d\n", ret);
		return ret;
	}
#elif CONFIG_LIBINETUTILS_TRACEROUTE_MAIN_FUNCTION
    ret = traceroute_main(argc, argv);
	if (ret) {
		fprintf(stderr, "traceroute main function failed. Exiting with code %d\n", ret);
		return ret;
	}
#endif

#ifdef CONFIG_INETUTILS_PROMETHEUS_SUPPORT
	ret = uk_prom_clean();
	if (ret) {
		fprintf(stderr, "Failed on lib-prometheus cleanup. Exiting with code %d\n", ret);
		return ret;
	}
#endif

	return 0;
}
