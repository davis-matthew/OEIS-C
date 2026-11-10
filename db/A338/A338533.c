/*
 * A338533  Primes in A338529/2.
 * https://oeis.org/A338533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k/2 for the n-th k (in order) in A338529 where k/2 is
 *            prime; A338529(m) = prime(m+2)*prime(m+3) - prime(m)*prime(m+1).
 *            Computed by a scan with a prime sieve and a primality test
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 9090 (2026-11-08)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_u64.h"

static uint64_t *A338533_pr = NULL;
static size_t A338533_pc = 0;
static idx_t A338533_lim = 0;

static uint64_t *A338533_found = NULL;
static size_t A338533_fcnt = 0;
static size_t A338533_fcap = 0;
static uint64_t A338533_scan = 1;   /* next m of A338529 to examine */

static void A338533_need(uint64_t cnt)
{
    while (A338533_pc < cnt) {
        idx_t nl = A338533_lim ? A338533_lim * 2 : (idx_t)(1u << 20);
        if (A338533_pr) free(A338533_pr);
        A338533_pr = NULL;
        A338533_pc = 0;
        build_primes_upto(nl, &A338533_pr, &A338533_pc);
        A338533_lim = nl;
    }
}

static void A338533_push(uint64_t v)
{
    if (A338533_fcnt == A338533_fcap) {
        size_t nc = A338533_fcap ? A338533_fcap * 2 : 1024;
        uint64_t *np = (uint64_t *)realloc(A338533_found, nc * sizeof(uint64_t));
        A338533_found = np;
        A338533_fcap = nc;
    }
    A338533_found[A338533_fcnt++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A338533_fcnt < (size_t)n) {
        uint64_t m = A338533_scan;
        A338533_need(m + 3);
        /* 1-based prime(j) = A338533_pr[j-1] */
        uint64_t pm  = A338533_pr[m - 1];
        uint64_t pm1 = A338533_pr[m];
        uint64_t pm2 = A338533_pr[m + 1];
        uint64_t pm3 = A338533_pr[m + 2];
        uint64_t v = pm2 * pm3 - pm * pm1;
        if ((v & 1) == 0) {
            uint64_t h = v / 2;
            if (is_prime_u64(h)) A338533_push(h);
        }
        A338533_scan++;
    }
    return (seq_t)A338533_found[(size_t)n - 1];
}
