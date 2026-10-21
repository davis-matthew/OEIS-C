/*
 * A266915  Power and add: distinct numbers a^b + c^d + e^f + g^h + i^j where
 *          a..j are permutations of 0..9.
 * https://oeis.org/A266915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9507, u64 n <= 9507, u128 n <= 9507,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A266915 – “Power and add”
   Compute all values a^b + c^d + e^f + g^h + i^j where the digits a..j
   form a permutation of 0..9.  The sequence is the ascending list of
   distinct sums.  The routine enumerates the 10! permutations, evaluates
   each sum, stores the results, sorts them, removes duplicates, and
   returns the n‑th value (1‑based).  The list contains 17 641 distinct
   terms. */
#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, free, qsort */

/* Comparator for qsort – works for any seq_t type (arbitrary‑precision
   classes or built‑in unsigned types). */
static int seq_cmp(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* Compute the next lexicographic permutation of arr[0..n-1].
   Returns non‑zero if a next permutation exists, otherwise 0. */
static int next_permutation(int *arr, int n)
{
    int i = n - 2;
    while (i >= 0 && arr[i] > arr[i + 1]) i--;
    if (i < 0) return 0;
    int j = n - 1;
    while (arr[j] < arr[i]) j--;
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
    int l = i + 1, r = n - 1;
    while (l < r) {
        tmp = arr[l];
        arr[l] = arr[r];
        arr[r] = tmp;
        l++; r--;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* One‑time static data */
    static seq_t *sums = NULL;
    static size_t total_unique = 0;
    static int initialized = 0;

    if (!initialized) {
        const idx_t TOTAL_PERM = 3628800ULL;      /* 10! */

        /* Pre‑compute base^exp for 0 <= base,exp <= 9 */
        seq_t pow_tbl[10][10];
        for (int b = 0; b <= 9; ++b) {
            for (int e = 0; e <= 9; ++e) {
                seq_t val = 1;
                for (int k = 0; k < e; ++k) val *= (seq_t)b;
                pow_tbl[b][e] = val;
            }
        }

        /* Allocate space for all 10! sums */
        sums = (seq_t *)malloc(TOTAL_PERM * sizeof(seq_t));
        if (!sums) return 0;      /* failure – return 0 */

        int perm[10];
        for (int i = 0; i < 10; ++i) perm[i] = i;
        idx_t idx = 0;
        do {
            seq_t s = pow_tbl[perm[0]][perm[1]]
                    + pow_tbl[perm[2]][perm[3]]
                    + pow_tbl[perm[4]][perm[5]]
                    + pow_tbl[perm[6]][perm[7]]
                    + pow_tbl[perm[8]][perm[9]];
            sums[idx++] = s;
        } while (next_permutation(perm, 10));

        /* Sort all sums */
        qsort(sums, TOTAL_PERM, sizeof(seq_t), seq_cmp);

        /* Remove duplicates in place */
        size_t uniq = 0;
        for (size_t i = 0; i < TOTAL_PERM; ++i) {
            if (i == 0 || sums[i] != sums[uniq - 1]) {
                sums[uniq++] = sums[i];
            }
        }
        total_unique = uniq;
        initialized = 1;
    }

    if (n == 0 || n > total_unique) return 0;    /* out of bounds */
    return sums[n - 1];
}
