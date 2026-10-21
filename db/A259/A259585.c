/*
 * A259585  Numbers k such that [r[s*k]] - [s[r*k]] = -1, where r = sqrt(2),
 *          s=sqrt(3), and [ ] = floor.
 * https://oeis.org/A259585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 2341, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259585: numbers k such that floor(sqrt(2)*floor(sqrt(3)*k)) -
   floor(sqrt(3)*floor(sqrt(2)*k)) = -1.
   The function generates the terms by an incremental brute‑force
   search and caches already found k's to answer subsequent calls
   efficiently. */
#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n) {
    static seq_t *list = NULL;
    static size_t list_len = 0;
    static size_t list_cap = 0;
    static idx_t last_k = 0;

    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1, but harmless */

    while (list_len < n) {
        ++last_k;
        seq_t kk = (seq_t)last_k;

        /* a = floor(sqrt(3)*k)  -> floor_sqrt(3*k^2) */
        seq_t a = floor_sqrt((seq_t)3 * kk * kk);
        /* b = floor(sqrt(2)*k)  -> floor_sqrt(2*k^2) */
        seq_t b = floor_sqrt((seq_t)2 * kk * kk);

        /* left = floor(sqrt(2)*floor(sqrt(3)*k)) -> floor_sqrt(2*a^2) */
        seq_t left  = floor_sqrt((seq_t)2 * a * a);
        /* right = floor(sqrt(3)*floor(sqrt(2)*k)) -> floor_sqrt(3*b^2) */
        seq_t right = floor_sqrt((seq_t)3 * b * b);

        /* Condition left + 1 == right ensures the difference is -1. */
        if (left + (seq_t)1 == right)
            append_candidate(&list, &list_len, &list_cap, kk);
    }

    return list[n - 1];
}
