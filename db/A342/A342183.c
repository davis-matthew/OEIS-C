/*
 * A342183  Primes q such that 15*q-2 and 15*q+2 are cousin primes.
 * https://oeis.org/A342183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime q such that 15*q-2 and 15*q+2 are both primes.
   Computed by successive prime testing (no closed‑form formula). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *seq_cache = NULL;     /* stored a(k) values               */
static idx_t seq_len   = 0;          /* number of values stored so far  */
static idx_t seq_cap   = 0;          /* allocated capacity of seq_cache*/
static idx_t next_candidate = 2;     /* next integer to test for q      */

/* Ensure that seq_cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n <= seq_len)
        return;

    while (seq_len < n)
    {
        /* Find the next prime q starting from next_candidate. */
        idx_t q = next_candidate;
        while (!is_prime_big((seq_t)q) && q != 0)   /* uint64_t wrap guard */
            ++q;
        if (q == 0)                                /* overflow, stop */
            break;
        next_candidate = q + 1;

        /* Compute 15*q ± 2. */
        seq_t q_t   = q;                /* convert to seq_t */
        seq_t v15q  = q_t * 15;         /* 15*q   */
        seq_t cand1 = v15q - 2;         /* 15*q-2 */
        seq_t cand2 = v15q + 2;         /* 15*q+2 */

        if (is_prime_big(cand1) && is_prime_big(cand2))
        {
            /* Grow the cache if necessary. */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, seq_cache, seq_cap);
                if (!tmp) exit(EXIT_FAILURE);
                seq_cache = tmp;
            }
            seq_cache[seq_len++] = q_t;   /* store the qualifying prime q */
        }
    }
}

/* External entry point demanded by the checker */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined below offset */
    ensure_cache(n);
    return seq_cache[n - 1];
}
