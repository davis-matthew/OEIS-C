/*
 * A342778  Positions of words in A342753 in which the last digit is 0.
 * https://oeis.org/A342778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k >= 1 such that the word w(k) of A342753 ends in 0;
 *            w(1)="0", w(3)="1", else w(k) ends like w(child(k)),
 *            child(k)=s'(k) if 3∤k else t'(k). Computed by a direct scan over
 *            k
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A342778_child(idx_t n)
{
    idx_t a = n - 1;
    idx_t tp = a / 3;
    idx_t sp = a - tp;
    return (n % 3 == 0) ? tp : sp;
}

static int A342778_last_is_zero(idx_t k)
{
    for (;;) {
        if (k == 1) return 1;
        if (k == 3) return 0;
        k = A342778_child(k);
    }
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (A342778_last_is_zero(k)) ++count;
    }
    return (seq_t)k;
}
