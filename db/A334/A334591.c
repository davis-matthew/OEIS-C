/*
 * A334591  Side length of largest triangle of zeros in the XOR-triangle with
 *          first row generated from the binary expansion of n.
 * https://oeis.org/A334591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) equals the side length of the largest sub‑triangle of zeros
 *            in the XOR‑triangle built from the binary expansion of n. The
 *            program counts all possible bases of such triangles and returns
 *            the maximum side length
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* bit length of n */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp >>= 1;
        ++L;
    }

    /* maximum bits is 64 for uint64_t */
    unsigned char rows[65][65];
    size_t len[65];

    /* top row - bits of n from most significant to least */
    len[0] = (size_t)L;
    for (size_t i = 0; i < len[0]; ++i) {
        rows[0][i] = (unsigned char)((n >> ((idx_t)(len[0] - 1 - i))) & 1);
    }

    /* subsequent rows of the XOR‑triangle */
    for (size_t r = 1; r < len[0]; ++r) {
        len[r] = len[r - 1] - 1;
        for (size_t i = 0; i < len[r]; ++i) {
            rows[r][i] = rows[r - 1][i] ^ rows[r - 1][i + 1];
        }
    }

    /* array of consecutive zeros to the right starting at each column */
    size_t zeros[65][66];
    for (size_t r = 0; r < len[0]; ++r) {
        zeros[r][len[r]] = 0;
        for (int i = (int)len[r] - 1; i >= 0; --i) {
            if (rows[r][i] == 0)
                zeros[r][i] = zeros[r][i + 1] + 1;
            else
                zeros[r][i] = 0;
        }
    }

    /* search for the largest zero triangle */
    seq_t best = 0;
    for (size_t r = 0; r < len[0]; ++r) {
        for (size_t j = 0; j < len[r]; ++j) {
            if (rows[r][j] == 0) {
                size_t s = 1;
                while (r >= s - 1 && zeros[r - (s - 1)][j] >= s) {
                    ++s;
                }
                seq_t side = (seq_t)(s - 1);
                if (side > best) best = side;
            }
        }
    }
    return best;
}
