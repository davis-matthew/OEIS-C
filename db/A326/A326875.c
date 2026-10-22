/*
 * A326875  BII-numbers of set-systems that are closed under union.
 * https://oeis.org/A326875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer (starting with n = 1) whose set of
 *            binary indices is closed under bitwise OR
 * Range:     u8 n <= 62, u16 n <= 2481, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Return true iff the integer n satisfies the BII property
   (the set of positions of its 1‑bits is closed under bitwise OR). */
static bool is_bii(idx_t n)
{
    uint64_t t = (uint64_t)n;
    if (t == 0)
        return true;

    /* positions of set bits – 0‑based */
    uint8_t pos[64];
    size_t sz = 0;
    for (uint8_t i = 0; i < 64; ++i) {
        if ((t >> i) & 1ULL)
            pos[sz++] = i;
    }

    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = 0; j < sz; ++j) {
            uint8_t a = pos[i];
            uint8_t b = pos[j];
            uint8_t idx_a = a + 1;
            uint8_t idx_b = b + 1;
            uint8_t u = idx_a | idx_b;   /* 1 .. 64 */
            if (u == 0 || u > 64)
                return false;
            uint8_t check = u - 1;       /* 0‑based bit to test */
            if (!((t >> check) & 1ULL))
                return false;
        }
    }
    return true;
}

/* nth term of A326875 (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t next = 0;          /* next number to test */

    /* generate terms until seq_len >= n */
    while (seq_len < (size_t)n) {
        if (is_bii(next)) {
            if (seq_len >= seq_cap) {
                size_t newcap = seq_cap ? seq_cap * 2 : 8;
                seq_t *newarr = (seq_t *)malloc(newcap * sizeof(seq_t));
                if (!newarr)
                    abort();
                if (seq)
                    memcpy(newarr, seq, seq_len * sizeof(seq_t));
                free(seq);
                seq = newarr;
                seq_cap = newcap;
            }
            seq[seq_len++] = (seq_t)next;
        }
        ++next;
    }

    return seq[(size_t)n - 1];
}
