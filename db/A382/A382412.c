/*
 * A382412  Numbers with no zeros in their base-7 representation.
 * https://oeis.org/A382412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑7 representation
 *            contains no digit 0. The implementation maps (n‑1) to base‑6
 *            digits 0‑5, adds 1 to each digit to obtain base‑7 digits 1‑6,
 *            and interprets the result in base‑7
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, return 0 */
    
    seq_t sum   = 0;        /* Σ_{k=1}^{L‑1} 6^k   */
    seq_t pow6  = 1;        /* 6^k              */
    idx_t L     = 0;        /* length of the base‑7 number */
    
    /* Find minimal L such that Σ_{k=1}^{L} 6^k >= n */
    do {
        pow6 *= (seq_t)6;   /* pow6 = 6^L */
        sum   += pow6;      /* sum = Σ_{k=1}^{L} 6^k */
        ++L;
    } while (sum < (seq_t)n);
    
    seq_t sum_prev = sum - pow6;                 /* Σ_{k=1}^{L‑1} 6^k */
    seq_t offset   = (seq_t)n - sum_prev - 1;     /* 0‑based offset in block */
    
    /* digits of offset in base‑6 with exactly L places (least significant first) */
    const int MAXL = 128;
    int digits[MAXL];
    for (idx_t i = 0; i < L; ++i) {
        digits[i] = (int)(offset % (seq_t)6);
        offset   /= (seq_t)6;
    }
    
    /* Build the result by interpreting digits +1 (1–6) as base‑7 */
    seq_t result = 0;
    for (idx_t i = L; i-- > 0; ) {
        result = result * (seq_t)7 + (seq_t)(digits[i] + 1);
    }
    return result;
}
