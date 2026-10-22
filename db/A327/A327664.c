/*
 * A327664  a(n) is the least base >= 2 such that for any k in the range
 *          0..n-1, the sum of digits of k and the sum of digits of n differ
 *          for at least one base b in the range 2..a(n).
 * https://oeis.org/A327664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest base b ≥ 2 such that for every k with 0 ≤ k
 *            < n there exists a base b' in the range 2 ≤ b' ≤ b for which the
 *            sum of digits of k and the sum of digits of n (both taken in
 *            base b') differ. The algorithm computes the minimal such b
 * Range:     u8 n <= 873, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is deterministic; the function works for all n ≥ 0
    (a(0) = 2).  It uses only integer arithmetic and standard C
    facilities.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* helper: sum of digits of v in the given base (base ≥ 2) */
static seq_t digit_sum_base(seq_t v, int base)
{
    seq_t sum = 0;
    while (v > 0) {
        sum += v % base;
        v   /= base;
    }
    return sum;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Trivial case */
    if (n == 0) return (seq_t)2;

    idx_t N = n;                  /* keep a local copy of n            */
    /* array marking which k still have equal digit‑sums in all bases
       processed so far (badAll[k] == true  ⇒  still “bad”) */
    bool *bad = (bool *)malloc(sizeof(bool) * N);
    if (!bad) return 0;           /* allocation failed – should not happen */

    for (idx_t i = 0; i < N; ++i) bad[i] = true;

    for (idx_t base = 2;; ++base) {
        seq_t sum_n = digit_sum_base((seq_t)N, (int)base);

        /* Update bad flags */
        for (idx_t k = 0; k < N; ++k) {
            if (!bad[k]) continue;                /* already good */
            seq_t sum_k = digit_sum_base((seq_t)k, (int)base);
            if (sum_k != sum_n) bad[k] = false;   /* differs → not bad */
        }

        /* Check whether all numbers are now good */
        bool anyBad = false;
        for (idx_t k = 0; k < N; ++k) {
            if (bad[k]) { anyBad = true; break; }
        }
        if (!anyBad) {
            free(bad);
            return (seq_t)base;
        }

        /* Safety guard – the definition guarantees a(n) ≤ n+1 */
        if (base > N + 1) {
            free(bad);
            return (seq_t)(N + 1);
        }
    }
}
