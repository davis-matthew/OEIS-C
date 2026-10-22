/*
 * A399782  (1/72)*number of ways to express n as the determinant of a 4 X 4
 *          matrix containing all permutations of the multiset {1^4, 2^4, 3^4,
 *          4^4}.
 * https://oeis.org/A399782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (1/72) * the number of 4×4 matrices whose entries are
 *            exactly four 1's, four 2's, four 3's, and four 4's and whose
 *            determinant equals n. The function evaluates this value for any
 *            n ≥ 0
 * Range:     u8 none, u16 none, u32 n <= 160, u64 n <= 160, u128 n <= 160,
 *            big any n
 * Helpers:   det4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/det4.h"

#define MAXDET   8192                /* safe bound for determinant range [-8192,8192] */
#define HIST_SZ  (2 * MAXDET + 1)
#define HIST_OFF (size_t)MAXDET

static seq_t hist[HIST_SZ] = {0};   /* counts of determinants */
static bool precomputed = false;   /* pre‑computation done? */

/* Generate the next lexicographic permutation of an array of 16 ints
   containing the multiset {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4}. */
static bool next_perm(int *arr, int n)
{
    int i = n - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) i--;
    if (i < 0) return false;                 /* last permutation reached */

    int j = n - 1;
    while (arr[j] <= arr[i]) j--;

    int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;

    /* reverse the suffix */
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = arr[l]; arr[l] = arr[r]; arr[r] = tmp;
        l++; r--;
    }
    return true;
}

/* Precompute the histogram of determinants.  This is run once on the first
   call to a(). */
static void precompute(void)
{
    int mat[16] = {1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4};
    do {
        int d = det4(mat);
        if (d >= -MAXDET && d <= MAXDET) {
            hist[(size_t)(d + MAXDET)] += (seq_t)1;
        }
    } while (next_perm(mat, 16));
    precomputed = true;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (!precomputed) {
        precompute();
    }
    if (n > (idx_t)MAXDET) {
        return (seq_t)0;
    }
    return hist[(size_t)(n + MAXDET)] / (seq_t)72;
}
