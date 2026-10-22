/*
 * A364362  Consider all the ways to make sequences of distinct nonnegative
 *          integers using all the digits of n, such that no term has leading
 *          0's or appears more than once in the sequence. a(n) is the minimum
 *          possible sum of any sequence of n.
 * https://oeis.org/A364362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1664, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A364362:
 *  a(n) = minimum possible sum of a sequence of distinct nonnegative integers
 *  that uses all decimal digits of n exactly once, with no leading zeros
 *  and no term repeated.
 *
 *  Implementation: brute‑force over all permutations of the digits of n
 *  and over all ways of partitioning each permutation into contiguous runs.
 *  For each partition we check the two constraints and keep the minimal
 *  sum found.  The algorithm is O(d! * 2^(d-1)) where d = number of
 *  decimal digits of n (d ≤ 5 for n ≤ 10000), which is trivial.
 *
 *  We avoid a sentinel value that could be negative under arbitrary‑precision
 *  types by using a “first” flag.
 */

#include <stdbool.h>
#include <string.h>

/*--- Helper: next permutation of an array of uint8_t ----------------------------------*/
static bool next_permutation(uint8_t *a, size_t n)
{
    if (n <= 1) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    uint8_t tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
    /* reverse tail */
    size_t l = i + 1, r = n - 1;
    while (l < r) {
        tmp = a[l];
        a[l] = a[r];
        a[r] = tmp;
        l++; r--;
    }
    return true;
}

/*--- Helper: insertion sort of uint8_t array ----------------------------------------*/
static void sort_uint8(uint8_t *a, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        uint8_t key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1] > key) {
            a[j] = a[j - 1];
            j--;
        }
        a[j] = key;
    }
}

/*--- Main sequence function --------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Convert n to decimal string */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%llu", (unsigned long long)n);
    if (len <= 0) return (seq_t)0;      /* safety */

    /* Extract digits */
    uint8_t digits[20];
    for (int i = 0; i < len; ++i) digits[i] = (uint8_t)(buf[i] - '0');
    size_t L = (size_t)len;

    /* Prepare sorted permutation array */
    uint8_t perm[20];
    memcpy(perm, digits, L);
    sort_uint8(perm, L);

    seq_t best = (seq_t)0;     /* will be overwritten on first hit */
    bool first = true;

    /* Enumerate all permutations */
    do {
        /* Number of partition masks = 2^(L-1) */
        size_t maxMask = (L == 1) ? 1 : (1ULL << (L - 1));
        for (size_t mask = 0; mask < maxMask; ++mask) {
            seq_t numbers[20];
            size_t pn = 0;
            seq_t cur = 0;
            size_t start = 0;
            bool invalid = false;

            /* Build numbers from the current partition */
            for (size_t i = 0; i < L; ++i) {
                cur = cur * (seq_t)10 + (seq_t)perm[i];
                if (i == L - 1 || (mask & (1ULL << i))) {
                    size_t segLen = i - start + 1;
                    if (segLen > 1 && perm[start] == 0) {  /* leading zero */
                        invalid = true;
                        break;
                    }
                    numbers[pn++] = cur;
                    cur = 0;
                    start = i + 1;
                }
            }
            if (invalid) continue;

            /* Check distinctness */
            bool duplicate = false;
            for (size_t i = 0; i < pn && !duplicate; ++i)
                for (size_t j = i + 1; j < pn; ++j)
                    if (numbers[i] == numbers[j]) { duplicate = true; break; }
            if (duplicate) continue;

            /* Compute sum and compare */
            seq_t sum = 0;
            for (size_t i = 0; i < pn; ++i) sum += numbers[i];
            if (first || sum < best) { best = sum; first = false; }
        }
    } while (next_permutation(perm, L));

    return best;
}
