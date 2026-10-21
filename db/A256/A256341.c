/*
 * A256341  Numbers which have only digits 8 and 9 in base 10.
 * https://oeis.org/A256341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1022, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A256341: nth number made only of digits 8 and 9 in increasing order.
   Compute the number of digits k from n, find the offset of the
   desired number within that digit length, then map binary bits
   (0→8, 1→9) to obtain the result. */
seq_t a(idx_t n)
{
    /* Determine k = floor(log2(n+1)) = number of digits. */
    idx_t x = n + 1;
    idx_t bitlen = 0;
    while (x) {
        ++bitlen;
        x >>= 1;
    }
    idx_t k = bitlen - 1;

    /* Count of numbers with fewer digits: 2^k - 2. */
    idx_t prev = ((idx_t)1 << k) - 2;

    /* 0‑based offset within the group of k‑digit numbers. */
    idx_t offset = n - prev - 1;

    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t bit = (offset >> (k - 1 - i)) & 1ULL;
        result = result * (seq_t)10 + (seq_t)(bit ? 9 : 8);
    }
    return result;
}
