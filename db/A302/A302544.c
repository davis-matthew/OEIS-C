/*
 * A302544  Lexicographically earliest sequence of distinct nonnegative
 *          numbers such that for any n >= 0, A065359(a(n)) = - A065359(n).
 * https://oeis.org/A302544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   lexicographically earliest self‑inverse permutation a(n) of
 *            non‑negative integers such that A065359(a(n)) = -A065359(n). The
 *            function builds the permutation on demand using a greedy choice,
 *            storing the result in static arrays
 * Range:     u8 n <= 255, u16 n <= 8630, u32 n <= 8630, u64 n <= 8630,
 *            u128 n <= 8630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Alternate sum of binary digits, starting with a + sign for
   the least significant bit. */
static int alt_sum(idx_t x)
{
    int sum = 0;
    int sign = 1;
    while (x) {
        if (x & (idx_t)1) sum += sign;
        sign = -sign;
        x >>= 1;
    }
    return sum;
}

/* ------------------------------------------------------------ */
/*  Parameters for the fixed‑size array that keeps the next
   candidate for each sum.  For n up to 10 000 the sum of
   alternating binary digits never exceeds 14 in magnitude,
   but a generous buffer is used here. */
#define SUM_OFFSET 2000
#define SUM_SIZE   (2 * SUM_OFFSET + 1)

/* ------------------------------------------------------------ */
/*  Static state shared by all calls. */
static bool          initialized = false;
static bool         *used        = NULL;
static idx_t        *pair_of     = NULL;          /* partner of each number */
static size_t        array_len   = 0;              /* number of indices we can address */
static idx_t        processed    = (idx_t)-1;      /* highest index processed so far */

/*  Stores the next unused candidate for each alternating‑digit sum. */
static idx_t next_candidate[SUM_SIZE] = {0};

/* ------------------------------------------------------------ */
static void ensure_capacity(idx_t N)
{
    if (N < array_len) return;                     /* already large enough */

    size_t new_len = array_len ? array_len : 256;
    while (new_len <= (size_t)N) new_len <<= 1;    /* grow geometrically */

    bool  *new_used  = (bool *)realloc(used,  new_len * sizeof(bool));
    idx_t *new_pair  = (idx_t *)realloc(pair_of, new_len * sizeof(idx_t));
    if (!new_used || !new_pair) abort();           /* allocation failure */

    /* initialize new slots */
    for (size_t i = array_len; i < new_len; ++i) {
        new_used[i] = false;
        new_pair[i] = (idx_t)-1;
    }

    used   = new_used;
    pair_of = new_pair;
    array_len = new_len;
}

/* ------------------------------------------------------------ */
static void init_once(void)
{
    if (initialized) return;

    array_len = 256;
    used      = (bool *)calloc(array_len, sizeof(bool));
    pair_of   = (idx_t *)calloc(array_len, sizeof(idx_t));
    if (!used || !pair_of) abort();

    for (size_t i = 0; i < array_len; ++i) pair_of[i] = (idx_t)-1;
    /* next_candidate already zero‑initialized as a global array */

    initialized = true;
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_once();
    ensure_capacity(n);                 /* need pair_of[n] after this */

    /* Process all indices that have not yet been paired. */
    idx_t start = (processed == (idx_t)-1) ? 0 : processed + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (pair_of[i] != (idx_t)-1) continue;   /* already paired */

        int    alt    = alt_sum(i);
        int    target = -alt;
        int    idx    = target + SUM_OFFSET;

        if (idx < 0 || idx >= SUM_SIZE) abort();  /* should not happen */

        idx_t candidate = next_candidate[idx];

        /* Find the smallest unused number with the required sum. */
        while (true) {
            ensure_capacity(candidate);          /* make sure we can test this index */
            if (!used[candidate] && alt_sum(candidate) == target) break;
            ++candidate;
        }

        /* Record the involutive pairing (a(i) = candidate). */
        pair_of[i] = candidate;
        pair_of[candidate] = i;
        used[i] = true;
        used[candidate] = true;

        next_candidate[idx] = candidate + 1;   /* next search starts after this */
    }

    processed = n;
    return (seq_t)pair_of[n];
}
