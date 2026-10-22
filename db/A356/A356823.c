/*
 * A356823  Tribternary numbers.
 * https://oeis.org/A356823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Tribternary numbers.
   a(n) is the n‑th number whose ternary representation uses only digits 0 and 1
   and does not contain three consecutive 1's.
   The function enumerates such numbers in increasing order. */
#include <stdlib.h>
#include <stddef.h>

static seq_t (*C)[3] = NULL;   /* C[rem][c] : number of ways to finish rem bits with trailing ones c */
static seq_t *CUM   = NULL;    /* CUM[L]  : total number of admissible strings of length ≤ L */
static size_t C_size = 0;      /* current allocated size of C and CUM */
static int initialized = 0;

static void ensure_len(size_t L)
{
    if (!initialized) {
        /* base case: length 0 */
        C = (seq_t (*)[3])malloc(sizeof(*C));
        CUM = (seq_t*)malloc(sizeof(seq_t));
        for (int c = 0; c < 3; ++c) C[0][c] = 1;
        CUM[0] = 0;
        C_size = 1;
        initialized = 1;
    }
    if (C_size > L) return;

    size_t new_size = L + 1;
    if (C_size < new_size) {
        C = (seq_t (*)[3])realloc(C, new_size * sizeof(*C));
        CUM = (seq_t*)realloc(CUM, new_size * sizeof(seq_t));
        /* compute new entries */
        for (size_t t = C_size; t <= L; ++t) {
            if (t == 0) continue;          /* already initialized above */
            for (int c = 0; c < 3; ++c) {
                seq_t count = C[t - 1][0];
                if (c < 2) {
                    count += C[t - 1][c + 1];
                }
                C[t][c] = count;
            }
            seq_t a_t = C[t - 1][1];        /* strings of length t that start with 1 */
            CUM[t] = CUM[t - 1] + a_t;
        }
        C_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 */

    seq_t idx = (seq_t)(n - 2);            /* zero‑based index among non‑zero terms */

    /* find minimal length L with CUM[L] > idx */
    size_t L = 1;
    while (1) {
        ensure_len(L);
        if (CUM[L] > idx) break;
        ++L;
    }
    seq_t prev_cum = (L > 1) ? CUM[L - 1] : (seq_t)0;
    seq_t offset  = idx - prev_cum;

    /* construct the lexicographically offset‑th string of length L */
    char *bits = (char*)malloc(L);
    bits[0] = 1;
    size_t rem = L - 1;
    int trailing = 1;
    for (size_t pos = 1; pos < L; ++pos) {
        for (int b = 0; b <= 1; ++b) {
            if (b == 1 && trailing == 2) continue;           /* would create '111' */
            int new_trailing = (b == 0) ? 0 : trailing + 1;
            seq_t count = C[rem - 1][new_trailing];
            if (offset < count) {
                bits[pos] = b;
                trailing = new_trailing;
                rem -= 1;
                break;
            } else {
                offset -= count;
            }
        }
    }

    /* evaluate the binary string as a ternary number */
    seq_t value = 0;
    for (size_t i = 0; i < L; ++i) {
        value = value * ((seq_t)3) + (seq_t)bits[i];
    }
    free(bits);
    return value;
}
