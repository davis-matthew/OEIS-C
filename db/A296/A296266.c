/*
 * A296266  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)*b(n), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2)*b(n) Initial values: a(0) = 1,
 *            a(1) = 2, b(0) = 3, b(1) = 4, b(2) = 5, (a(n)) and (b(n)) are
 *            increasing complementary sequences
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* binary search for a seq_t in a sorted array */
static bool seq_contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static storage for the sequences */
    static seq_t *A = NULL, *B = NULL;
    static size_t Alen = 0, Blen = 0;
    static size_t Acap = 0, Bcap = 0;
    static bool init = false;

    if (!init) {
        Acap = 8;          /* initial capacity for A */
        Bcap = 8;          /* initial capacity for B */
        A = (seq_t *)malloc(Acap * sizeof(seq_t));
        B = (seq_t *)malloc(Bcap * sizeof(seq_t));
        if (!A || !B) return (seq_t)0;   /* allocation failure */

        /* seed the known initial terms */
        A[Alen++] = (seq_t)1;            /* a(0) */
        A[Alen++] = (seq_t)2;            /* a(1) */

        B[Blen++] = (seq_t)3;            /* b(0) */
        B[Blen++] = (seq_t)4;            /* b(1) */
        B[Blen++] = (seq_t)5;            /* b(2) */

        init = true;
    }

    /* Expand sequences up to n (inclusive) */
    for (size_t i = Alen; i <= (size_t)n; ++i) {
        /* Ensure b(i) exists */
        while (Blen <= i) {
            seq_t cand = B[Blen - 1] + (seq_t)1;
            while (seq_contains(A, Alen, cand))
                ++cand;

            if (Blen >= Bcap) {
                size_t new_cap = Bcap ? Bcap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                B = tmp;
                Bcap = new_cap;
            }
            B[Blen++] = cand;
        }

        /* Compute a(i) */
        seq_t val = A[i - 1] + A[i - 2] + B[i - 2] * B[i];

        if (Alen >= Acap) {
            size_t new_cap = Acap ? Acap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            A = tmp;
            Acap = new_cap;
        }
        A[Alen++] = val;
    }

    return A[(size_t)n];
}
