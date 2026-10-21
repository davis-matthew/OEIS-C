/*
 * A259207  5x + 1 sequence beginning at 5.
 * https://oeis.org/A259207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 5; a(n)=5*a(n-1)+1 if a(n-1) odd else a(n-1)/2
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t cache[20];
    static unsigned char init = 0;
    if (!init) {
        cache[0] = 5;
        for (idx_t i = 1; i < 20; ++i) {
            seq_t prev = cache[i-1];
            if (prev & 1) {
                cache[i] = 5 * prev + 1;
            } else {
                cache[i] = prev >> 1;
            }
        }
        init = 1;
    }
    if (n < 10) return cache[n];
    idx_t pos = 10 + (n % 10);
    return cache[pos];
}
