/*
 * A266604  Integers whose writing includes only the vowel "e" (in French).
 * https://oeis.org/A266604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, fini, full
 * Formula:   a(n) = h + t + u where h∈{0,100,700}, t∈{0,30}, u∈{0,7}
 *            (excluding 0); the resulting numbers are sorted ascending,
 *            offset 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t seq_table[12];
static bool seq_ready = false;

static void build_seq(void) {
    /* Generate all combinations of the allowed parts. */
    static const seq_t hundreds[3] = {0, 100, 700};
    static const seq_t tens[2]     = {0, 30};
    static const seq_t units[2]    = {0, 7};

    seq_t vals[12];
    size_t cnt = 0;
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 2; ++j)
            for (size_t k = 0; k < 2; ++k)
                vals[cnt++] = hundreds[i] + tens[j] + units[k];

    /* Simple insertion sort */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = vals[i];
        size_t j = i;
        while (j > 0 && vals[j - 1] > key) {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    /* Store in table with offset 1 */
    for (size_t i = 0; i < cnt; ++i)
        seq_table[i] = vals[i];

    seq_ready = true;
}

seq_t a(idx_t n) {
    if (!seq_ready)
        build_seq();

    /* seq_table[0] == 0, seq_table[1] == 7, ... */
    if (n >= 12)
        return (seq_t)0; /* undefined beyond known terms */

    return seq_table[n];
}
