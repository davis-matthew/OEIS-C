/*
 * A280446  Like A065759 but where f(n) = 2*n.
 * https://oeis.org/A280446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   For n ≥ 5, let l = floor((n‑5)/3) and t = (n‑5) mod 3. * Then *
 *            if t = 0: a(n) = 394062 × 10^l * if t = 1: a(n) = 5848 ×
 *            10^(l+2) * if t = 2: a(n) = 6884 × 10^(l+2) * (preceding terms:
 *            a(1)=5848, a(2)=6884, a(3)=58480, a(4)=68840)
 * Range:     u8 none, u16 n <= 3, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    switch (n) {
        case 1: return (seq_t)5848;
        case 2: return (seq_t)6884;
        case 3: return (seq_t)5848 * pow10((idx_t)1); /* 58480 */
        case 4: return (seq_t)6884 * pow10((idx_t)1); /* 68840 */
    }

    /* For n ≥ 5: compute using the pattern described above. */
    idx_t idx = n - 5;          /* zero‑based index in the tail sequence */
    idx_t l   = idx / 3;        /* length offset (l = 0 for numbers of length 6) */
    idx_t t   = idx % 3;        /* position within the three numbers of that length */

    if (t == 0) {
        /* 394062 × 10^l */
        return (seq_t)394062 * pow10(l);
    } else {
        /* 5848 or 6884 times 10^(l+2) */
        seq_t base = (t == 1) ? (seq_t)5848 : (seq_t)6884;
        return base * pow10(l + 2);
    }
}
