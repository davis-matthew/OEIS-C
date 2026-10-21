/*
 * A248057  Positions of 1,1 in the Thue-Morse sequence (A010060).
 * https://oeis.org/A248057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = positions i (1‑based) where Thue‑Morse(i)=1 and
 *            Thue‑Morse(i+1)=1
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1 */

    idx_t i = 1;           /* current index (1‑based) */
    idx_t found = 0;       /* number of pairs found so far */

    while (1) {
        /* Thue‑Morse value at index i is parity of bits of (i-1) */
        if (__builtin_parityll((unsigned long long)(i - 1)) &&
            __builtin_parityll((unsigned long long)i)) {
            ++found;
            if (found == n)
                return (seq_t)i;
        }
        ++i;
    }
}
