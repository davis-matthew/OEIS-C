/*
 * A309109  Number of possible permutations of a Pyraminx of size n,
 *          disregarding the trivial rotation of the tips.
 * https://oeis.org/A309109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 272097792 * 369600^(2*n-6) * a(n-3) for n >= 6, with a(1)
 *            = 1, a(2) = 1, a(3) = 933120. For n < 6 the values are given
 *            directly
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function big_pow is provided in the utilities, so we include its header. */
#include "../utils/big_pow.h"

/* Helper to compute the formula for small n (1 .. 5). */
static seq_t _a_small(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)933120;

    /* Exponents derived from the two closed‑form expressions. */
    seq_t exp2;
    seq_t exp3;
    seq_t exp1925;

    if (n % 3 == 0) {
        /* 3 | n  :  a(n) = 5 * 2^(2 n^2-3 n-1) *
                        3^(n^2/3 + 3 n-6) * 1925^(n^2/3-n) */
        exp2    = (seq_t)(2 * n * n - 3 * n - 1);
        exp3    = (seq_t)(n * n / 3 + 3 * n - 6);
        exp1925 = (seq_t)(n * n / 3 - n);
    } else {
        /* 3 ∤ n :  a(n) = 5 * 2^(2 n^2-3 n-1) *
                        3^(n^2/3 + 3 n-16/3) *
                        1925^(n^2/3-n-1/3) */
        exp2    = (seq_t)(2 * n * n - 3 * n - 1);
        exp3    = (seq_t)((n * n + 9 * n - 16) / 3);
        exp1925 = (seq_t)((n * n - 3 * n - 1) / 3);
    }

    seq_t val = (seq_t)5;
    val = val * big_pow((seq_t)2, exp2);
    val = val * big_pow((seq_t)3, exp3);
    val = val * big_pow((seq_t)1925, exp1925);
    return val;
}

seq_t a(idx_t n)
{
    /* Handle the known small values directly. */
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)933120;
    if (n == 4 || n == 5) return _a_small(n);

    /* n >= 6 : we use the recurrence in steps of 3. */
    idx_t r = n % 3;
    if (r == 0) r = 3;          /* map residue 0 to 3 */
    idx_t start = r;
    while (start < 6) start += 3;   /* first index >= 6 with the same residue */

    /* Value of a(start-3), which is known for start <= 8. */
    seq_t cur_val;
    if (start == 6)          cur_val = (seq_t)933120;
    else if (start == 7)     cur_val = _a_small(4);  /* a(4) */
    else                     cur_val = _a_small(5);  /* a(5) */

    /* Current power 369600^(2*idx-6), starting from idx = start. */
    seq_t pow_cur = big_pow((seq_t)369600, (seq_t)(2 * start - 6));

    /* One big power that we multiply each step: 369600^6. */
    seq_t pow6 = big_pow((seq_t)369600, (seq_t)6);

    /* Iterate over the indices from start to n stepping by 3. */
    for (idx_t idx = start; idx <= n; idx += 3) {
        /* Apply the recurrence: a(idx) = 272097792 * 369600^(2*idx-6) * a(idx-3) */
        cur_val = cur_val * (seq_t)272097792 * pow_cur;
        /* Update the power for the next step. */
        pow_cur = pow_cur * pow6;
    }

    return cur_val;
}
