/*
 * A367067  a(1)=3, thereafter a(n) is the least positive integer not yet in
 *          the sequence such that Sum_{i=1..n} a(i) == 3 (mod n+3).
 * https://oeis.org/A367067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367067: a(1)=3; for n>1 a(n) = least unused positive integer such that
   Σ_{i=1..n} a(i) ≡ 3 (mod n+3). */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for already computed sequence elements and used values */
    static seq_t *seq = NULL;        /* computed sequence values */
    static size_t seq_sz = 0;        /* number of terms stored    */
    static size_t seq_cap = 0;       /* allocated capacity of seq */

    static seq_t *used_vals = NULL;  /* values already chosen          */
    static size_t used_sz = 0;        /* number of used values           */
    static size_t used_cap = 0;       /* allocated capacity of used_vals */

    static seq_t running_sum = (seq_t)0;  /* Σ a(i) for all computed terms */

    /* Grow seq array if needed. */
    if (seq_cap == 0) {
        seq_cap = 64;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    }
    while (seq_cap < n) {
        size_t new_cap = seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        seq = tmp;
        seq_cap = new_cap;
    }

    /* Grow used_vals array if needed. */
    if (used_cap == 0) {
        used_cap = 64;
        used_vals = (seq_t *)calloc(used_cap, sizeof(seq_t));
    }
    while (used_sz == used_cap) {
        size_t new_cap = used_cap * 2;
        seq_t *tmp = (seq_t *)realloc(used_vals, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        used_vals = tmp;
        used_cap = new_cap;
    }

    /* Return already computed term. */
    if (seq_sz >= n) return seq[n - 1];

    /* Compute missing terms up to n. */
    for (size_t k = seq_sz + 1; k <= n; ++k) {
        /* Modulus for term k: m = k + 3 */
        seq_t m = (seq_t)(k + 3);

        /* Find remainder r that makes (running_sum + r) % m == 3. */
        seq_t target = (seq_t)3;
        seq_t sum_mod = running_sum % m;
        seq_t r;
        if (sum_mod <= target) {
            r = target - sum_mod;
        } else {
            r = target + m - sum_mod;     /* ensures r < m */
        }

        /* First candidate with this remainder, guaranteed positive. */
        seq_t cand = (r == (seq_t)0) ? m : r;

        /* Search for the smallest unused candidate in the residue class. */
        while (1) {
            bool used = false;
            for (size_t i = 0; i < used_sz; ++i) {
                if (used_vals[i] == cand) {
                    used = true;
                    break;
                }
            }
            if (!used) break;
            cand += m;
        }

        /* Record the chosen value. */
        seq[k - 1] = cand;
        used_vals[used_sz++] = cand;
        running_sum += cand;
        seq_sz = k;
    }

    return seq[n - 1];
}
