/*
 * A356148  a(n) is the number of positive integers whose binary expansion
 *          appears as a substring in the binary expansion of n or its
 *          complement.
 * https://oeis.org/A356148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the number of distinct positive integers whose binary *
 *            representation occurs as a substring of n or of its bitwise
 *            complement * (with the same bit-length, leading zeros ignored)
 * Range:     u8 n <= 311, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void add_unique(seq_t val, seq_t *list, int *cnt)
{
    for (int i = 0; i < *cnt; ++i)
        if (list[i] == val) return;
    list[(*cnt)++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0 */

    /* number of bits in n */
    int len = 0;
    uint64_t tmp = n;
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* bits of n, most‑significant bit first */
    unsigned char bits[64];
    for (int i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1ULL);

    /* array of seen values (size is far larger than needed) */
    seq_t seen[5000];
    int seen_cnt = 0;

    /* enumerate substrings of the original binary string */
    for (int i = 0; i < len; ++i) {
        if (bits[i] == 0) continue;   /* skip substrings starting with 0 */
        seq_t val = 0;
        for (int j = i; j < len; ++j) {
            val = (val << 1) | (seq_t)bits[j];
            add_unique(val, seen, &seen_cnt);
        }
    }

    /* enumerate substrings of the complement (bits flipped) */
    for (int i = 0; i < len; ++i) {
        if ((bits[i] ^ 1ULL) == 0) continue;   /* skip 0 in complement */
        seq_t val = 0;
        for (int j = i; j < len; ++j) {
            val = (val << 1) | (seq_t)(bits[j] ^ 1ULL);
            add_unique(val, seen, &seen_cnt);
        }
    }

    return (seq_t)seen_cnt;
}
