/*
 * A267456  Binary representation of the n-th iteration of the "Rule 133"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A267456.
   For n=0  → 1
   For n=1  → 10
   For n=2  → 100
   For n≥3:
        – if n is even:  a(n) is the decimal integer obtained by
          concatenating (n‑2) copies of the two‑digit string "10"
          followed by the three‑digit string "100".
        – if n is odd :  a(n) is the decimal integer obtained by
          concatenating (n‑3) copies of the two‑digit string "10"
          followed by the four‑digit string "1000".  */

seq_t a(idx_t n)
{
    /* Handle the trivial low‑index cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)100;

    seq_t val = (seq_t)0;

    if ((n & 1) == 0) {            /* even n  (n ≥ 2) */
        idx_t repeats = n - 2;      /* number of "10" blocks to prepend */
        for (idx_t i = 0; i < repeats; ++i)
            val = val * (seq_t)100 + (seq_t)10;  /* append "10" */
        val = val * (seq_t)1000 + (seq_t)100;    /* append "100" */
    } else {                       /* odd n  (n ≥ 3) */
        idx_t repeats = n - 3;      /* number of "10" blocks to prepend */
        for (idx_t i = 0; i < repeats; ++i)
            val = val * (seq_t)100 + (seq_t)10;  /* append "10" */
        val = val * (seq_t)10000 + (seq_t)1000; /* append "1000" */
    }

    return val;
}
