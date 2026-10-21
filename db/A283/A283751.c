/*
 * A283751  Least b>1 such that n, when expressed in base b and then
 *          interpreted in base b^2, yields a multiple of n.
 * https://oeis.org/A283751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 3138, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search:  a(n) = least base b>1 such that the base‑b representation of n,
   read as a number in base b², is a multiple of n.  */
/* Author: ChatGPT, October 2026 */

seq_t a(idx_t n) {
    if (n == 0)                 /* Spec: a(0) = 2 */
        return 2;

    for (idx_t b = 2; ; ++b) {  /* try bases in increasing order          */
        idx_t  temp = n;        /* copy of n to extract base‑b digits      */
        seq_t  mod   = 0;        /* (value in base b²) modulo n            */
        seq_t  pow   = 1 % n;    /* (b²)⁰ mod n                            */
        seq_t  mult  = ((seq_t)b * (seq_t)b) % n;  /* (b²) mod n */

        while (temp) {          /* process each digit of n in base b       */
            idx_t digit = temp % b;
            temp /= b;

            mod += (seq_t)digit * pow;
            mod %= n;

            pow = (pow * mult) % n;
        }

        if (mod == 0)           /* condition satisfied: n | value          */
            return b;
    }
}
