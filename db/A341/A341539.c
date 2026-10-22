/*
 * A341539  One of the two successive approximations up to 2^n for the 2-adic
 *          integer sqrt(17). This is the 3 (mod 4) case.
 * https://oeis.org/A341539
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 64, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A341539 – 2‑adic integer sqrt(17) (3 mod 4)  
   a(2) = 3  
   for n ≥ 3: a(n) = a(n−1)  if (a(n−1)^2 − 17) is divisible by 2^(n+1)  
             a(n) = a(n−1) + 2^(n−1)  otherwise  
   The values are kept in `seq_t`, an unsigned integer type supplied by the build. */
seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 2 – return 0 otherwise. */
    if (n < 2) {
        return 0;
    }

    /* Start with the initial value a(2) = 3. */
    seq_t val = (seq_t)3;

    /* pow2i will hold 2^i during the loop.  Start with 1 = 2^0. */
    seq_t pow2i = 1;

    /* Iterate up to the requested n, applying the recurrence. */
    for (idx_t i = 1; i <= n; ++i) {
        /* Advance to 2^i. */
        pow2i <<= 1;                     /* 2^i */

        /* The recurrence begins at i = 3 (i.e., n ≥ 3). */
        if (i >= 3) {
            /* Compute a^2 – 17 in unsigned arithmetic. */
            seq_t diff = val * val - (seq_t)17;

            /* We need to test divisibility by 2^(i+1).  Because the modulus
               is a power of two, this is equivalent to checking the low
               (i+1) bits.  For unsigned arithmetic, the low bits are
               obtained by masking with (2^(i+1) – 1). */
            seq_t mask = (pow2i << 1) - 1;          /* 2^(i+1) – 1 */

            /* If the low bits are zero, the condition holds. */
            if ((diff & mask) != 0) {
                /* Add the required offset 2^(i−1).  Since pow2i == 2^i,
                   pow2i >> 1 == 2^(i−1). */
                val += (pow2i >> 1);
            }
        }
    }

    /* The final `val` is a(n). */
    return val;
}
