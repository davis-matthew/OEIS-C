/*
 * A262574  Concatenation of the numbers from 1 to n but omitting 4.
 * https://oeis.org/A262574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the first n terms of the sequence
 *            1,2,3,5,6,7,8,9,10,11,12,13,14,15,... (omit the integer 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    static const seq_t pow10[6] = { (seq_t)1, (seq_t)10, (seq_t)100, (seq_t)1000,
                                    (seq_t)10000, (seq_t)100000 };
    for (idx_t i = 1; i <= n; ++i) {
        /* The i‑th term of the sequence that omits 4 has value i
           for i=1,2,3 and i+1 for i≥4. */
        seq_t term = (i <= 3) ? (seq_t)i : (seq_t)(i + 1);

        /* Determine the number of decimal digits of term. */
        unsigned int digits;
        if (term >= 10000)          digits = 5;
        else if (term >= 1000)      digits = 4;
        else if (term >= 100)       digits = 3;
        else if (term >= 10)        digits = 2;
        else                        digits = 1;

        /* Append the term to the current result. */
        result = result * pow10[digits] + term;
    }
    return result;
}
