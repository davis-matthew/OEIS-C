/*
 * A332756  A loop sequence within Pi. Let a(1) = 19. For n > 1, a(n+1) is the
 *          position of the first occurrence of a(n) after the decimal point
 *          in the decimal expansion of Pi.
 * https://oeis.org/A332756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = { 19 if n mod 3 = 1; 37 if n mod 3 = 2; 46 if n mod 3 = 0
 *            } (The sequence has period 3.)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t cycle[3] = {19, 37, 46};
    if (n == 0) return 0; /* undefined for n<1, but return 0 as a safe default */
    return cycle[(n - 1) % 3];
}
