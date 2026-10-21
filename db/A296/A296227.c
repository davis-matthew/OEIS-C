/*
 * A296227  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0) - n, where a(0) = 1, a(1) = 2,
 *          b(0) = 3, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   expand_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{i=0}^{n-1} a(i)*b(n-1-i) – n
 * Initial values:
 *     a(0) = 1, a(1) = 2
 *     b(0) = 3
 * a and b are increasing complementary sequences.
 *
 * This implementation builds the sequences incrementally, keeping a
 * running convolution array so that each a(n) is obtained in O(n) time
 * overall.  It uses only integer arithmetic allowed by the
 * specifications and is fully self‑contained.
 */

#include <stdlib.h>   /* malloc, realloc, calloc, exit */
#include <stdint.h>
#include "../utils/expand_seq.h"

/* forward declarations of helper utilities that may be provided */
static void expand_seq(seq_t **arr, size_t *cap, size_t needed);
static size_t count_used_leq(seq_t x);
static seq_t kth_unused_b(size_t k);

/* static storage for the sequences */
static seq_t *A = NULL, *B = NULL, *C = NULL;   /* C = convolution */
static size_t a_len = 0, b_len = 0;
static size_t a_cap = 0, b_cap = 0, conv_cap = 0;

/* Binary search: number of elements of A that are ≤ x. */
static size_t count_used_leq(seq_t x)
{
    size_t low = 0, high = a_len;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (A[mid] <= x) low = mid + 1;
        else             high = mid;
    }
    return low;
}

/* Return the (k)-th unused positive integer (0‑based),
 * i.e. the smallest integer not in A. */
static seq_t kth_unused_b(size_t k)
{
    /* lower bound after the last b already found */
    seq_t lo = (b_len == 0) ? ((seq_t)2 + 1) : (B[b_len - 1] + 1);
    seq_t hi = lo;

    /* find an upper bound where the count of unused integers > k */
    while (1) {
        size_t used  = count_used_leq(hi);
        seq_t unused = hi - (seq_t)used;
        if (unused > (seq_t)k) break;
        hi <<= 1;
    }

    /* binary search for minimal x with unused > k */
    while (lo < hi) {
        seq_t mid = (lo + hi) >> 1;
        size_t used_mid  = count_used_leq(mid);
        seq_t unused_mid = mid - (seq_t)used_mid;
        if (unused_mid > (seq_t)k) hi = mid;
        else                      lo = mid + 1;
    }
    return lo;
}

/* Ensure sequences are initialized with the base terms. */
static void init_sequences(void)
{
    if (A != NULL) return;              /* already initialized */

    a_cap = 4;  A = (seq_t *)calloc(a_cap, sizeof(seq_t));
    b_cap = 4;  B = (seq_t *)calloc(b_cap, sizeof(seq_t));
    conv_cap = 1; C = (seq_t *)calloc(conv_cap, sizeof(seq_t));

    A[0] = (seq_t)1;
    A[1] = (seq_t)2;
    a_len = 2;

    /* b is generated lazily; b_len starts at 0 */
    b_len = 0;

    /* conv is initially empty; it will be filled when b[0] is generated */
}

/* Fill A[0..n] respecting the recurrence and complementary property. */
static void ensure_a(idx_t n)
{
    /* keep generating until we have a[n] ready */
    while (a_len <= (size_t)n) {

        /* first, ensure all b[0..a_len-1] exist */
        while (b_len <= a_len - 1) {
            seq_t nb = kth_unused_b(b_len);

            expand_seq(&B, &b_cap, b_len + 1);
            B[b_len] = nb;

            /* add contributions of the new b to convolution */
            for (size_t i = 0; i < a_len; ++i) {
                seq_t prod = A[i] * nb;
                size_t pos = i + b_len;     /* new b index is b_len before increment */
                expand_seq(&C, &conv_cap, pos + 1);
                C[pos] += prod;
            }
            ++b_len;
        }

        /* now compute a[a_len] using the convolution value */
        seq_t sum = C[a_len - 1];
        seq_t val = sum - (seq_t)a_len;

        expand_seq(&A, &a_cap, a_len + 1);
        A[a_len] = val;

        /* update convolution with the new a against all existing b */
        for (size_t j = 0; j < b_len; ++j) {
            seq_t prod = val * B[j];
            size_t pos = a_len + j;          /* new a index is a_len before increment */
            expand_seq(&C, &conv_cap, pos + 1);
            C[pos] += prod;
        }
        ++a_len;
    }
}

/* Public function required by the judge */
seq_t a(idx_t n)
{
    init_sequences();
    ensure_a(n);
    return A[n];
}
