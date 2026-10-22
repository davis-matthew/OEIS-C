/*
 * A330366  Prime numbers p_k such that p_k == 1 (mod 10) and p_(k+1) == 1
 *          (mod 10).
 * https://oeis.org/A330366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 225, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p_k with p_k ≡ 1 (mod 10) and p_(k+1) ≡ 1 (mod 10) */
/* Implementation: on first call (or when more terms are needed) we
 * grow a sieve to generate all primes up to a limit, then walk
 * through the prime list, keeping each prime whose successor also
 * satisfies the congruence.  The list is cached statically, so
 * subsequent calls return values immediately.  The sieve size is
 * doubled until enough terms have been gathered.  This is O(N log log N)
 * per call for the first few terms and very fast for n ≤ 10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* static cache */
static seq_t *primes_list      = NULL; /* holds the sequence terms */
static size_t primes_list_cnt  = 0;    /* number of terms stored */
static uint64_t sieve_limit    = 2000000; /* initial sieving bound */
static bool cache_ready = false;

/* helper: rebuild the list using the current sieve_limit */
static void rebuild_list(void)
{
    /* obtain all primes up to the current limit */
    uint64_t *primes = NULL;
    size_t  prime_cnt = 0;
    build_primes_upto(sieve_limit, &primes, &prime_cnt);

    /* temporary array for the sequence */
    size_t cap = 64;
    seq_t *tmp = (seq_t*)malloc(cap * sizeof(seq_t));
    size_t cnt = 0;

    /* scan adjacent primes; keep the first when both ≡1 mod 10 */
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        if ((primes[i]      % 10ULL) == 1ULL &&
            (primes[i + 1] % 10ULL) == 1ULL)
        {
            if (cnt == cap) {
                cap <<= 1;
                tmp = (seq_t*)realloc(tmp, cap * sizeof(seq_t));
            }
            tmp[cnt++] = (seq_t)primes[i];
        }
    }
    free(primes);

    /* replace old cache */
    free(primes_list);
    primes_list     = tmp;
    primes_list_cnt = cnt;
    cache_ready     = true;
}

seq_t a(idx_t n)
{
    /* undefined for n = 0 – guard against accidental use. */
    if (n == 0) return (seq_t)0;

    /* build or enlarge the cache until it contains at least n terms */
    while (!cache_ready || primes_list_cnt < (size_t)n) {
        if (!cache_ready) {
            rebuild_list();                /* first construction */
        } else {
            sieve_limit *= 2;              /* enlarge search bound */
            rebuild_list();
        }
    }

    return primes_list[n - 1];
}
