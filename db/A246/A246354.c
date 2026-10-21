/*
 * A246354  Rectangular array: T(n,k) is the position in the infinite
 *          Fibonacci word s = A003849 at which the block s(1)..s(n) occurs
 *          for the k-th time.
 * https://oeis.org/A246354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(n_row, k_col) where (n_row, k_col) are obtained from n
 *            by mapping to the antidiagonal (n_row + k_col = s), with s the
 *            smallest integer such that s*(s-1)/2 ≥ n. T(n,k) is the position
 *            of the k-th occurrence of the prefix of length n in the infinite
 *            Fibonacci word
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes a sufficiently long prefix of the
   Fibonacci word and then, on demand, records the first 200 occurrences
   of each prefix.  The function a() works for any n ≥ 1 in any order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define WORD_LEN      2000000      /* length of Fibonacci word to precompute   */
#define MAX_ROW       2000         /* maximum n_row that will be requested     */
#define MAX_K        200           /* number of occurrences precomputed per row*/

static uint8_t  word[WORD_LEN + 2];      /* 1‑based indexing, values 0/1           */
static idx_t    word_len = 0;
static seq_t    occ[MAX_ROW + 1][MAX_K];
static int      occ_cnt[MAX_ROW + 1] = {0};

static bool initialized = false;

static void init_word(void)
{
    if (initialized) return;
    /* generate Fibonacci word up to WORD_LEN using iterative concatenation */
    static uint8_t f0[WORD_LEN + 2], f1[WORD_LEN + 2], f2[WORD_LEN + 2];
    size_t len0 = 1, len1 = 2;

    f0[1] = '0';
    f1[1] = '0'; f1[2] = '1';

    while (len1 < WORD_LEN) {
        size_t l2 = len1 + len0;
        if (l2 > WORD_LEN) l2 = WORD_LEN;
        size_t use1 = (len1 < l2) ? len1 : l2;
        memcpy(f2 + 1, f1 + 1, use1);
        size_t use0 = (l2 > len1) ? (l2 - len1) : 0;
        if (use0) memcpy(f2 + 1 + use1, f0 + 1, use0);

        /* shift f0 <- f1, f1 <- f2 */
        memcpy(f0 + 1, f1 + 1, len1);
        memcpy(f1 + 1, f2 + 1, l2);
        len0 = len1;
        len1 = l2;
    }
    /* copy to global word array */
    memcpy(word + 1, f1 + 1, len1);
    word_len = len1;
    initialized = true;
}

/* compute first MAX_K occurrences of prefix length n, store in occ[n] */
static void precompute_row(idx_t n)
{
    if (n > MAX_ROW) return;                     /* out of bound – safe guard */
    if (occ_cnt[n] != 0) return;                 /* already computed */

    int pi[ MAX_ROW + 1 ];
    int j = 0;
    pi[1] = 0;
    for (int i = 2; i <= (int)n; i++) {
        while (j > 0 && word[i] != word[j + 1]) j = pi[j];
        if (word[i] == word[j + 1]) j++;
        pi[i] = j;
    }

    j = 0;
    int found = 0;
    for (idx_t i = 1; i + n - 1 <= word_len && found < MAX_K; i++) {
        while (j > 0 && word[i] != word[j + 1]) j = pi[j];
        if (word[i] == word[j + 1]) j++;
        if (j == (int)n) {
            occ[n][found++] = (seq_t)(i - n + 1);
            j = pi[j];
        }
    }
    occ_cnt[n] = found;   /* may be less than MAX_K if word too short */
}

seq_t a(idx_t n)
{
    init_word();

    /* find diagonal s such that s*(s-1)/2 >= n */
    idx_t s = (idx_t)((1 + sqrtl((long double)1 + 8.0L *
                                 (long double)n)) / 2);
    while ((s - 1) * s / 2 < n) s++;
    while (s >= 2 && (s - 2) * (s - 1) / 2 >= n) s--;

    idx_t prev = (s - 2) * (s - 1) / 2;
    idx_t offset = n - prev;              /* 1 .. s-1 */
    idx_t n_row = offset;                 /* row index */
    idx_t k_col = s - offset;             /* column index */

    if (n_row > MAX_ROW) return 0;        /* out of precompute range */
    precompute_row(n_row);
    if (k_col == 0 || k_col > (idx_t)MAX_K || occ_cnt[n_row] < (int)k_col)
        return 0;                         /* position not found */

    return occ[n_row][k_col - 1];
}
