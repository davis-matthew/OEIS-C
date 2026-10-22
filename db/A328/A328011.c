/*
 * A328011  The 5x + 1 sequence beginning at 1.
 * https://oeis.org/A328011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n+1)= 5*a(n)+1 if a(n) odd, else a(n)/2.
   Periodic: a(n)=a(n-7) for n>6.
   Initial values: a(0)=1, a(1)=6, a(2)=3, a(3)=16, a(4)=8, a(5)=4, a(6)=2. */

seq_t a(idx_t n) {
    static const seq_t cycle[7] = { (seq_t)1, (seq_t)6, (seq_t)3, (seq_t)16, (seq_t)8, (seq_t)4, (seq_t)2 };
    return cycle[n % 7];
}
