/*
 * A332887  a(n) is the number of partition numbers > 1 that are proper
 *          divisors of the n-th partition number.
 * https://oeis.org/A332887
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:  For n ≥ 2 compute p(n), the integer partition number.
   Then return the number of indices k with 2 ≤ k < n such that
   p(k) divides p(n).  This counts the partition numbers > 1 that are
   proper divisors of the n‑th partition number. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* dynamic array that holds p(0)…p(part_last) */
static seq_t *part     = NULL;
static size_t part_cap = 0;
static idx_t  part_last = 0;

/* ensure that part[0..n] are computed (n is zero‑based) */
static void ensure_part(idx_t n)
{
    if (n <= part_last) return;                /* already computed */

    if (part_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(part, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        part     = tmp;
        part_cap = (size_t)(n + 1);
    }

    if (part_last == 0) {                       /* initialise p(0)=p(1)=1 */
        part[0] = (seq_t)1;
        if (part_cap >= 2) part[1] = (seq_t)1;
        part_last = (part_cap >= 2) ? 1 : 0;
    }

    for (idx_t m = part_last + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * ((idx_t)3 * k - 1) / 2;
            if (pent1 > m) break;
            seq_t term = part[m - pent1];
            if ((k & 1) == 0) sum -= term;
            else              sum += term;

            idx_t pent2 = k * ((idx_t)3 * k + 1) / 2;
            if (pent2 > m) continue;
            term = part[m - pent2];
            if ((k & 1) == 0) sum -= term;
            else              sum += term;
        }
        part[m] = sum;
    }
    part_last = n;
}

/* the function required by the OEIS framework */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                 /* offset is 2 */

    ensure_part(n);

    seq_t count = (seq_t)0;
    seq_t p_n   = part[n];

    for (idx_t k = 2; k < n; ++k) {              /* skip p(0), p(1) and p(n) */
        if ((p_n % part[k]) == (seq_t)0) ++count;
    }
    return count;
}
