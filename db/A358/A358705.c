/*
 * A358705  Zeroless pandigital numbers whose square has each digit 1 to 9
 *          twice.
 * https://oeis.org/A358705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lookup: a(n) is the nth zeroless pandigital whose square uses
   digits 1..9 twice each. This function precomputes all such numbers
   by enumerating permutations of 1..9. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool built = false;

/* check whether a 9‑digit number has no zero and its square contains
   each digit 1–9 exactly twice. */
static bool is_valid(uint64_t num)
{
    unsigned __int128 sq = (unsigned __int128)num * num;
    unsigned char freq[10] = {0};
    while (sq > 0) {
        int d = (int)(sq % 10);
        freq[d]++;
        sq /= 10;
    }
    for (int d = 1; d <= 9; ++d)
        if (freq[d] != 2)
            return false;
    return freq[0] == 0;
}

/* C implementation of std::next_permutation for 9 elements. */
static bool next_perm(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] > a[i + 1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] < a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; ++l, --r) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

/* generate all terms once */
static void build_sequence(void)
{
    const int digits[9] = {1,2,3,4,5,6,7,8,9};
    int perm[9];
    for (int i = 0; i < 9; ++i) perm[i] = digits[i];

    /* max 9! permutations */
    cache = seq_alloc(362880);
    cache_len = 0;

    do {
        uint64_t num = 0;
        for (int i = 0; i < 9; ++i)
            num = num * 10 + perm[i];
        if (is_valid(num))
            cache[cache_len++] = (seq_t)num;
    } while (next_perm(perm, 9));

    built = true;
}

seq_t a(idx_t n)
{
    if (!built)
        build_sequence();

    if (n == 0) return 0;
    if (n > cache_len) return 0;  /* out of range */
    return cache[n - 1];
}
