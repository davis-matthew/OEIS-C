/*
 * A366421  a(n) is the floor of the n-th improper fraction (including the
 *          ones of the type n/n) sorted by increasing numerator+denominator,
 *          then by numerator.
 * https://oeis.org/A366421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(A138099(n)/(A216607(n)+1))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition of sequence A366421: the floor of the n‑th improper fraction
   (including n/n) sorted by increasing numerator+denominator, then by numerator. */

seq_t a(idx_t n) {
    /* Find the sum S for which the n-th fraction lies */
    idx_t s = 2;                      /* current numerator+denominator sum */
    idx_t cumulative = 0;             /* number of fractions with sum < s */

    while (1) {
        idx_t count = s / 2;          /* number of admissible pairs for this sum */
        idx_t next = cumulative + count;
        if (next >= n) break;        /* n-th fraction is in this sum */
        cumulative = next;
        s++;                          /* move to next sum */
    }

    /* Index of the fraction within this sum (0‑based) */
    idx_t offset = n - cumulative - 1;

    /* Compute the numerator and denominator of the fraction */
    idx_t start = (s + 1) / 2;        /* smallest numerator for this sum: ceil(s/2) */
    idx_t i = start + offset;        /* numerator of the nth fraction */
    idx_t j = s - i;                  /* denominator: j = s - i */

    /* Return the floor of i/j (integer division) */
    return (seq_t)i / (seq_t)j;
}
