/*
 * A316849  The table in A316841 with columns concatenated to form a single
 *          number.
 * https://oeis.org/A316849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is formed by concatenating the components of the
 * triple (a,b,c) that appears in the A316841 table.
 * The table lists all positive integer triples satisfying  
 *     a ≥ b ≥ c ≥ a – b + 1                               
 * and they are ordered first by a, then by b, then by c.
 * a(n) is the decimal concatenation of a,b and c for the nth such triple.
 * For example a(1)=111, a(2)=221, a(3)=222, a(4)=322, … */

#include <stdint.h>

/* Helper: number of decimal digits of x. */
static int digit_len(seq_t x)
{
    int len = 0;
    while (x > 0) {
        x /= 10;
        ++len;
    }
    return len;
}

/* Helper: 10^e as a seq_t. */
static seq_t pow10(idx_t e)
{
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 10;
    return res;
}

/* Return the nth term of A316849 (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Find the value of a for which the cumulative count reaches n. */
    idx_t a_val   = 1;   /* current a being examined */
    idx_t cum     = 0;   /* number of terms counted so far */

    while (1) {
        /* Number of triples with this a. */
        idx_t T;
        if ((a_val & 1) == 0) {          /* a = 2*m */
            idx_t m = a_val >> 1;       /* m = a / 2 */
            T = m * (m + 1);            /* m*(m+1) */
        } else {                          /* a = 2*m+1 */
            idx_t m = a_val >> 1;       /* m = floor(a/2) */
            T = (m + 1) * (m + 1);      /* (m+1)^2 */
        }

        if (cum + T >= n)               /* we are in this a block */
            break;

        cum  += T;                       /* go to next a */
        ++a_val;
    }

    /* Offset within the current a's block (1‑based). */
    idx_t offset = n - cum;              /* 1 ≤ offset ≤ T */

    /* Lower bound for b: ceil((a+1)/2) = (a+2)/2 for all integers a. */
    idx_t b_lower = (a_val + 2) >> 1;     /* (a_val + 2) / 2 */

    /* Find the correct (b,c) pair. */
    idx_t b_val = 0, c_val = 0;
    for (idx_t b = b_lower; b <= a_val; ++b) {
        idx_t count_c = 2 * b - a_val;    /* number of allowed c values */
        if (offset > count_c) {
            offset -= count_c;
        } else {
            b_val = b;
            idx_t c_start = a_val - b + 1;    /* smallest c for this b */
            c_val = c_start + (offset - 1);
            break;
        }
    }

    /* Convert components to seq_t. */
    seq_t A = (seq_t)a_val;
    seq_t B = (seq_t)b_val;
    seq_t C = (seq_t)c_val;

    /* Concatenate: a * 10^(digits(b)+digits(c)) + b * 10^(digits(c)) + c */
    int db = digit_len(B);
    int dc = digit_len(C);
    seq_t pow_bc = pow10((idx_t)(db + dc));
    seq_t pow_c  = pow10((idx_t)dc);

    return A * pow_bc + B * pow_c + C;
}
