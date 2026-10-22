/*
 * A375231  Interleaving A006040 and A228229.
 * https://oeis.org/A375231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375231: interleaving A006040 and A228229.
   a(n) = A006040((n+1)/2)  for odd n
   a(n) = A228229(n/2)      for even n
   A228229(k) satisfies B[0] = 1,  B[n] = (n^2+n)*B[n-1] + 1
   A006040(k) satisfies S[1] = 1, S[2] = 9,  S[n] = n^2*S[n-1] + 1  (n≥3)   */
#include <stdlib.h>
#include <stddef.h>

static seq_t *even_seq = NULL;
static size_t   even_max = 0;   /* largest index computed in even_seq  */
static seq_t *odd_seq  = NULL;
static size_t   odd_max  = 0;   /* largest index computed in odd_seq   */

static void init_even(void)
{
    if (even_seq != NULL) return;
    even_seq = (seq_t*)calloc(1, sizeof(seq_t));   /* index 0 only */
    even_seq[0] = (seq_t)1;                        /* A228229(0) = 1 */
    even_max = 0;
}

static void init_odd(void)
{
    if (odd_seq != NULL) return;
    odd_seq = (seq_t*)calloc(3, sizeof(seq_t));    /* indices 0..2 used */
    odd_seq[0]  = (seq_t)0;                        /* unused   */
    odd_seq[1]  = (seq_t)1;                        /* A006040(1) = 1 */
    odd_seq[2]  = (seq_t)9;                        /* A006040(2) = 9 */
    odd_max = 2;
}

/* Extend even_seq up to index k (inclusive) using the recurrence
   B[n] = (n^2 + n) * B[n‑1] + 1.  */
static void grow_even(idx_t k)
{
    if (k <= even_max) return;
    size_t new_len = (size_t)(k + 1);
    even_seq = (seq_t*)realloc(even_seq, new_len * sizeof(seq_t));
    for (idx_t i = even_max + 1; i <= k; ++i) {
        seq_t prev = even_seq[i - 1];
        seq_t mult = (seq_t)i * ((seq_t)i + (seq_t)1);   /* n^2 + n */
        even_seq[i] = mult * prev + (seq_t)1;
    }
    even_max = k;
}

/* Extend odd_seq up to index k (inclusive) using the recurrence
   S[n] = n^2 * S[n‑1] + 1   (n ≥ 3; S[1]=1, S[2]=9). */
static void grow_odd(idx_t k)
{
    if (k <= odd_max) return;
    size_t new_len = (size_t)(k + 1);
    odd_seq = (seq_t*)realloc(odd_seq, new_len * sizeof(seq_t));
    for (idx_t i = odd_max + 1; i <= k; ++i) {
        seq_t prev = odd_seq[i - 1];
        seq_t mult = (seq_t)i * (seq_t)i;             /* n^2 */
        odd_seq[i] = mult * prev + (seq_t)1;
    }
    odd_max = k;
}

seq_t a(idx_t n)
{
    /* Even n: a(n) = A228229(n/2) */
    if ((n & (idx_t)1) == 0) {
        idx_t k = n / (idx_t)2;
        init_even();
        grow_even(k);
        return even_seq[k];
    }
    /* Odd n: a(n) = A006040((n+1)/2) */
    else {
        idx_t k = (n + (idx_t)1) / (idx_t)2;
        init_odd();
        grow_odd(k);
        return odd_seq[k];
    }
}
