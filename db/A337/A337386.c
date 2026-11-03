/*
 * A337386  Numbers k for which A003973(k) >= 2*A003961(k).
 * https://oeis.org/A337386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 25993, u64 n <= 25993,
 *            u128 n <= 25993, big any n
 * Validated: standard, pass, 72 terms through n = 9786 (2026-11-03)
 * Helpers:   factor_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by iterating over integers and selecting those whose
   prime‑shift once is an odd abundant number. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* Return true if the shift of n, obtained by replacing each prime factor
   p of n by the next prime > p while keeping the exponent, is an odd
   abundant number. */
static bool shift_abundant(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n; primes[i] to exps[i] form the factorization. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute shift(n) in 128‑bit integer to avoid overflow. */
    unsigned __int128 shift128 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t newp = next_prime(primes[i]);

        /* newp^exps[i] via binary exponentiation. */
        unsigned __int128 base = newp;
        uint64_t e = (uint64_t)exps[i];
        unsigned __int128 pow = 1;
        while (e) {
            if (e & 1)
                pow *= base;
            base *= base;
            e >>= 1;
        }
        shift128 *= pow;
    }
    uint64_t shift = (uint64_t)shift128;          /* fits in 64‑bit for our range */

    seq_t sigma = sigma_of(shift);                /* sum of all divisors */
    seq_t twice = (seq_t)(shift + shift);         /* 2 * shift */
    return sigma > twice;                        /* abundant test */
}

/* Return the n-th term of OEIS sequence A337386 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static idx_t cur = 1;        /* next integer to examine */

    while (len < n) {
        while (true) {
            if (shift_abundant(cur)) {
                append_candidate(&seq, &len, &cap, (seq_t)cur);
                ++cur;
                break;
            }
            ++cur;
        }
    }
    return seq[n - 1];
}
