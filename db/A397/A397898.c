/*
 * A397898  Values of the discriminant b^2 - 4*a*c of the quadratic equation
 *          a*x^2 + b*x + c = 0 for triples (a, b, c) of positive numbers,
 *          ordered first by their sum and then component-wise.
 * https://oeis.org/A397898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = b^2 - 4*a*c for the nth positive integer triple (a,b,c)
   ordered first by sum and then component‑wise.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined offset, but guard against it */

    /* Find smallest s such that  C(s,3) >= n  */
    idx_t s = 3;
    idx_t cum = 0;          /* C(s-1,3)   */
    while (1) {
        idx_t count = ((s - 1) * (s - 2)) / 2;  /* number of triples for sum s  */
        if (cum + count >= n) break;
        cum += count;
        ++s;
    }

    /* Offset within the block of triples having sum s  */
    idx_t offset = n - cum - 1;   /* 0‑based */

    /* Locate the triple (a,b,c) using the offset  */
    idx_t a_val = 0, b_val = 0, c_val = 0;
    for (idx_t a_i = 1; a_i <= s - 2; ++a_i) {
        idx_t num_b = s - a_i - 1;  /* number of possible b for this a_i */
        if (offset < num_b) {
            a_val = a_i;
            b_val = offset + 1;
            c_val = s - a_val - b_val;
            break;
        }
        offset -= num_b;
    }

    /* Compute the discriminant  b^2 - 4*a*c  in type seq_t  */
    seq_t a_seq = (seq_t)a_val;
    seq_t b_seq = (seq_t)b_val;
    seq_t c_seq = (seq_t)c_val;

    return b_seq * b_seq - (seq_t)4 * a_seq * c_seq;
}
