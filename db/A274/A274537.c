/*
 * A274537  Number T(n,k) of set partitions of [n] into k blocks such that
 *          each element is contained in a block whose index parity coincides
 *          with the parity of the element; triangle T(n,k), n>=0, 0<=k<=n,
 *          read by rows.
 * https://oeis.org/A274537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 125, u32 n <= 260, u64 n <= 606,
 *            u128 n <= 1556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A274537
 *  Read the triangle T(n,k) of set partitions into k blocks with parity
 *  constraints by rows.  a(n) is the n-th element of that flattened triangle.
 *  Computation is performed by iterating the triangular recurrence
 *  T(n,k) = T(n-1,k) · cntParity + [k>0 && (k%2)==(n%2)] · T(n-1,k-1),
 *  where cntParity is the number of blocks of the required parity among
 *  k existing blocks.  The recurrence uses only (+, *, integer divisions)
 *  and therefore works for any seq_t (built‑in integer or big integer).
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* locate the row r and column k that correspond to the
       flattened index n  (0‑based).  Row r satisfies
       r(r+1)/2 ≤ n < (r+1)(r+2)/2.                                     */
    idx_t r = 0;
    idx_t base = 0;
    while (1) {
        idx_t next = (r + 1) * (r + 2) / 2;
        if (n < next) break;
        r++;
        base = next;
    }
    idx_t k = n - base;

    /* allocate row 0 (T(0,0)=1) */
    seq_t *prev = NULL;
    size_t prev_len = 0;
#ifdef __cplusplus
    prev = (seq_t*)malloc(sizeof(seq_t));
    new (&prev[0]) seq_t();      /* default‑construct */
    prev[0] = 1;
    prev_len = 1;
#else
    prev = calloc(1, sizeof(seq_t));
    prev[0] = 1;
    prev_len = 1;
#endif

    /* build rows 1..r incrementally; only the previous and the current
       row are kept in memory at any time. */
    for (idx_t id = 1; id <= r; ++id) {
        size_t curr_len = (size_t)id + 1;
        seq_t *curr;
#ifdef __cplusplus
        curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
        for (size_t j = 0; j < curr_len; ++j) new (&curr[j]) seq_t();
#else
        curr = calloc(curr_len, sizeof(seq_t));
#endif

        int parity = (int)(id & 1);  /* parity of the new element */
        for (size_t j = 0; j < curr_len; ++j) {
            seq_t val = 0;

            /* contribution from existing block k=j */
            if (j < prev_len) {
                idx_t oddCnt  = (j + 1) / 2;
                idx_t evenCnt = j / 2;
                idx_t cntParity = (parity == 1) ? oddCnt : evenCnt;
                if (cntParity != 0)
                    val += prev[j] * (seq_t)cntParity;
            }

            /* contribution from starting a new block (hoisting element j) */
            if (j > 0 && ( (j & 1) == parity )) {
                val += prev[j - 1];
            }

            curr[j] = val;
        }

        /* destroy previous row and free its storage */
#ifdef __cplusplus
        for (size_t j = 0; j < prev_len; ++j) prev[j].~seq_t();
        free(prev);
#else
        free(prev);
#endif
        prev     = curr;
        prev_len = curr_len;
    }

    /* obtain the requested value and clean up the final row */
    seq_t result = prev[k];
#ifdef __cplusplus
    for (size_t j = 0; j < prev_len; ++j) prev[j].~seq_t();
    free(prev);
#else
    free(prev);
#endif
    return result;
}
