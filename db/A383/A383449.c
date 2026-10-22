/*
 * A383449  Number of permutations of length n which avoid the patterns 123
 *          and 4321.
 * https://oeis.org/A383449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383449: number of permutations of length n avoiding patterns 123 and 4321.
   Generating function: 1 + x + 2x^2 + 5x^3 + 13x^4 + 25x^5 + 25x^6
   Thus a(n) = 0 for n ≥ 7. */

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)2;
        case 3: return (seq_t)5;
        case 4: return (seq_t)13;
        case 5: return (seq_t)25;
        case 6: return (seq_t)25;
        default: return (seq_t)0;
    }
}
