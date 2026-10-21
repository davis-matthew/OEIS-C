/*
 * A289814  A binary encoding of the twos in ternary representation of n (see
 *          Comments for precise definition).
 * https://oeis.org/A289814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0)=0; a(3n)=2*a(n); a(3n+1)=2*a(n); a(3n+2)=2*a(n)+1;
 * Range:     u8 n <= 6560, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, a(n) is the binary number whose i‑th bit is 1 iff the i‑th ternary digit of n is 2. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t pos = 0;
    while (n > 0) {
        if (n % 3 == 2)
            res |= ((seq_t)1 << pos);
        n /= 3;
        pos++;
    }
    return res;
}
