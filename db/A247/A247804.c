/*
 * A247804  Numbers in decimal representation with distinct digits, such that
 *          in French their digits are in alphabetic order.
 * https://oeis.org/A247804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word, fini, full
 * Formula:   a(n) is the nth smallest number whose decimal digits are
 *            distinct * and sorted ascending according to the French
 *            alphabetic order of the * French names of digits: * 5 (cinq), 2
 *            (deux), 8 (huit), 9 (neuf), 4 (quatre), 7 (sept), * 6 (six), 3
 *            (trois), 1 (un), 0 (zéro)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1022, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Table of all 2^10-1=1023 numbers that satisfy the digit rule. */
static seq_t table[1023];
static bool initialized = false;

static void init(void)
{
    if (initialized) return;

    static const int order[10] = {5, 2, 8, 9, 4, 7, 6, 3, 1, 0};

    idx_t idx = 0;
    /* Generate all non‑empty subsets of digits {0,…,9}. */
    for (idx_t mask = 1; mask < (1ull << 10); ++mask) {
        seq_t val = 0;
        /* Append digits in the required French order. */
        for (int i = 0; i < 10; ++i) {
            if (mask & (1ull << i))
                val = val * 10 + order[i];
        }
        table[idx++] = val;
    }

    /* In‑place insertion sort on the 1023 generated numbers. */
    idx_t n = idx;
    for (idx_t i = 1; i < n; ++i) {
        seq_t key = table[i];
        long long j = i - 1;          /* signed index for loop backwards */
        while (j >= 0 && table[j] > key) {
            table[j + 1] = table[j];
            --j;
        }
        table[j + 1] = key;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    if (n == 0 || n > 1023) return 0;   /* undefined outside the known range */

    return table[n - 1];
}
