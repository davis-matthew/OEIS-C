/*
 * A335169  Nim n-th power of 10.
 * https://oeis.org/A335169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Periodic formula: a(n+5) = a(n) with initial terms a(0)=1, a(1)=10, a(2)=14, a(3)=13, a(4)=8.
 * The sequence is thus determined solely by n modulo 5.
 */
seq_t a(idx_t n)
{
    idx_t r = n % 5;
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)10;
        case 2: return (seq_t)14;
        case 3: return (seq_t)13;
        case 4: return (seq_t)8;
        default: return (seq_t)0; /* unreachable, but kept for completeness */
    }
}
