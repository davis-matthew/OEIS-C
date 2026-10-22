/*
 * A372974  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *          positive number such that a(n) is coprime to a(n-1) and
 *          omega(a(n)) does not equal omega(a(n-1)).
 * https://oeis.org/A372974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest unused
 *            positive integer m such that gcd(a(n‑1), m) = 1 and omega(m) ≠
 *            omega(a(n‑1)), where omega(m) is the number of distinct prime
 *            factors of m
 * Range:     u8 n <= 203, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"

/* number of distinct prime factors for a 64‑bit integer */
static int omega_uint64(uint64_t x)
{
    int cnt = 0;
    if (x < 2) return 0;
    if ((x & 1ULL) == 0) {
        ++cnt;
        while ((x & 1ULL) == 0) x >>= 1;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            ++cnt;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) ++cnt;
    return cnt;
}

/* dynamic storage for the sequence and for the used‑number bitmap */
static seq_t      *seq         = NULL;  /* sequence values */
static size_t     seq_len      = 0;      /* number of terms stored (next index = seq_len+1) */
static unsigned char *used      = NULL;    /* 0 = unused, 1 = used */
static size_t     used_len     = 0;      /* size of used[] */

/* enlarge used[] so that it can hold indices up to max (inclusive) */
static void extend_used_to(size_t max)
{
    if (max < used_len) return;
    unsigned char *new_used = (unsigned char*)realloc(used, max * sizeof(unsigned char));
    if (!new_used) exit(1);
    /* initialise new bytes to 0 */
    memset(new_used + used_len, 0, (max - used_len) * sizeof(unsigned char));
    used = new_used;
    used_len = max;
}

/* compute the next term after prev (with omega_prev) */
static seq_t compute_next(seq_t prev, int omega_prev)
{
    for (seq_t m = 1; ; ++m)
    {
        uint64_t m64 = (uint64_t)m;          /* used only for indexing and omega */
        size_t idx  = (size_t)m64;
        extend_used_to(idx + 1);
        if (used[idx]) continue;                         /* already used */
        if (gcd(prev, m) != (seq_t)1) continue;           /* not coprime */
        int omega_m = omega_uint64(m64);
        if (omega_m == omega_prev) continue;             /* same omega */
        used[idx] = 1;                                    /* mark as used */
        return m;                                         /* found next value */
    }
}

/* public interface required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n==0, return 0 */

    /* initialise sequence with a(1)=1, a(2)=2 */
    if (seq_len == 0)
    {
        extend_used_to(3);                     /* indices 0,1,2 */
        seq = (seq_t*)realloc(seq, 2 * sizeof(seq_t));
        if (!seq) exit(1);
        seq[0] = (seq_t)1;       /* a(1) */
        seq[1] = (seq_t)2;       /* a(2) */
        used[1] = 1;
        used[2] = 1;
        seq_len = 2;
    }

    /* compute further terms on demand (in any order of calls) */
    while (seq_len < (size_t)n)
    {
        seq_t  prev        = seq[seq_len - 1];
        int    omega_prev  = omega_uint64((uint64_t)prev);
        seq_t  next_val    = compute_next(prev, omega_prev);

        seq = (seq_t*)realloc(seq, (seq_len + 1) * sizeof(seq_t));
        if (!seq) exit(1);
        seq[seq_len] = next_val;
        ++seq_len;
    }

    return seq[n - 1];
}
