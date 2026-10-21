/*
 * A259897  a(n) is the 2-adic valuation of phi(binomial(2*n,n)).
 * https://oeis.org/A259897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = popcount(n)-1 + Σ_{p odd prime, p<2n, digits of n in base
 *            p > (p-1)/2} v₂(p-1)
 * Range:     u8 n <= 602, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is valid for any n>=0.  It is memoized with a
   dynamic sieve that grows only when a larger n is requested.   */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* static data for the prime sieve */
static uint32_t *primes = NULL;       /* list of odd primes (2 is omitted)   */
static unsigned *v2_arr = NULL;      /* v₂(p-1) for each prime in the list */
static size_t primes_cnt = 0;        /* number of primes stored           */
static idx_t   primes_limit = 0;     /* largest n we have sieved for      */

/* ------------------------------------------------------------------ */
/* ensure_primes(n) : make sure the prime tables contain all odd
   primes less than 2*n.  The sieve is rebuilt only when n increases.
   For small limits (<= 2*10⁴) this is trivial.                      */
static void ensure_primes(idx_t n)
{
    idx_t limit = 2*n - 1;                         /* primes < 2n are needed */
    if (limit <= primes_limit) return;            /* already sufficient    */

    /* free old memory if any (safe for first call) */
    free(primes);   free(v2_arr);

    size_t alloc_size = (size_t)(limit + 1);      /* plenty of space */
    primes   = (uint32_t*)malloc(alloc_size * sizeof(uint32_t));
    v2_arr   = (unsigned*)malloc(alloc_size * sizeof(unsigned));
    if (!primes || !v2_arr) { abort(); }        /* out of memory */

    bool *is_comp = (bool*)calloc(alloc_size, sizeof(bool));
    if (!is_comp) { abort(); }

    primes_cnt = 0;

    for (idx_t i = 3; i <= limit; i += 2) {
        if (!is_comp[i]) {
            primes[primes_cnt] = (uint32_t)i;
            v2_arr[primes_cnt] = __builtin_ctz((unsigned)(i-1)); /* v₂(p-1) */
            primes_cnt++;
            if (i * i <= limit) {
                for (idx_t j = i*i; j <= limit; j += i<<1) {
                    is_comp[j] = true;
                }
            }
        }
    }
    free(is_comp);
    primes_limit = limit;
}

/* ------------------------------------------------------------------ */
/* a(n) implementation */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(0)=a(1)=0 explicitly   */

    ensure_primes(n);              /* ensure primes < 2n are ready */

    /* popcount(n) via compiler builtin (works for 64‑bit types) */
    unsigned popcnt = __builtin_popcountll((unsigned long long)n);
    seq_t result = (seq_t)(popcnt - 1);   /* first part of formula   */

    /* second part: iterate over odd primes < 2n */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((idx_t)p >= 2*n) break;  /* stop when reaching the limit */

        /* check whether any base‑p digit of n exceeds (p-1)/2 */
        idx_t  temp = n;
        unsigned threshold = (p-1) >> 1;     /* (p-1)/2 */
        bool found = false;
        while (temp) {
            if ((temp % p) > threshold) {
                found = true; break;
            }
            temp /= p;
        }
        if (found) {
            result += (seq_t)v2_arr[i];    /* add v₂(p-1) */
        }
    }
    return result;
}
