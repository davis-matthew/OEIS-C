/*
 * A336891  Sequence following Van Eck's sequence unless a prime which was
 *          previously encountered is found, in which case, a(n+1) = n - m,
 *          where m is the largest possible value such that a(m) is also
 *          prime.
 * https://oeis.org/A336891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0. For n ≥ 1, let curr = a(n). If curr is a prime which
 *            has appeared before (< n), then a(n+1) = n - m where m is the
 *            largest index < n such that a(m) is prime. Otherwise, if curr
 *            has appeared before, a(n+1) = n - m where m is the largest index
 *            < n with a(m) = curr. If curr has not appeare...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *seq          = NULL; /* 1‑based array of sequence terms */
    static idx_t capacity      = 0;    /* allocated size of seq             */
    static idx_t len           = 0;    /* number of terms that have been
                                         * computed (max index present)     */
    static idx_t prev_prime    = 0;    /* largest prime index < current n   */

    if (n == 0) return 0;          /* safety: sequence is defined for n>=1 */

    /* allocate initial storage if needed */
    if (seq == NULL) {
        capacity = 8;
        seq = mem_new(seq_t, capacity);
        /* initialise first term */
        seq[1] = (seq_t)0;
        len    = 1;
        prev_prime = 0;             /* a(1) is not prime */
    }

    /* extend capacity if target n is beyond current allocation */
    if (capacity <= n) {
        idx_t new_cap = capacity;
        while (new_cap <= n) new_cap <<= 1;
        seq = mem_resize(seq_t, seq, new_cap);
        capacity = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t current = seq[i - 1];   /* a(n) for current i-1 */

        /* find last index j < i-1 where seq[j] == current */
        idx_t last_same = 0;
        for (idx_t j = i - 2; j > 0; --j) {
            if (seq[j] == current) {
                last_same = j;
                break;
            }
        }

        bool current_is_prime = is_prime_big(current);
        seq_t next;

        if (current_is_prime && last_same > 0) {
            /* repeated prime: use the most recent prime index */
            next = (seq_t)(i - 1) - (seq_t)prev_prime;
        } else if (last_same > 0) {
            /* repeated non‑prime value */
            next = (seq_t)(i - 1) - (seq_t)last_same;
        } else {
            /* first occurrence of current value */
            next = (seq_t)0;
        }

        seq[i] = next;

        /* update prev_prime for future iterations */
        if (current_is_prime) {
            prev_prime = i - 1;
        }

        ++len;
    }

    return seq[n];
}
