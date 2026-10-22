/*
 * A308230  Irregular triangle: row n shows the alternating sums of partitions
 *          of n when the parts are arranged in nonincreasing order and the
 *          partitions are arranged lexicographically from [n] to
 *          [1,1,1,...,1].
 * https://oeis.org/A308230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the alternating sum of the parts of the partition that *
 *            occupies the n‑th position when all integer partitions of m ≥ 1
 *            are listed * lexicographically (parts non‑increasing) and
 *            concatenated row by row. * The function computes the row r
 *            containing n, then generates the * pos‑th partition...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static void gen_partitions(int remaining, int max_part, int depth,
                           idx_t target, idx_t *counter, seq_t *res,
                           int *found, int *parts)
{
    if (remaining == 0) {
        (*counter)++;
        if (*counter == target) {
            seq_t alt = 0;
            for (int i = 0; i < depth; ++i) {
                if (i & 1)
                    alt -= (seq_t)parts[i];
                else
                    alt += (seq_t)parts[i];
            }
            *res = alt;
            *found = 1;
        }
        return;
    }
    for (int part = max_part; part >= 1; --part) {
        if (part > remaining)
            continue;
        parts[depth] = part;
        gen_partitions(remaining - part, part, depth + 1,
                       target, counter, res, found, parts);
        if (*found)
            return;
    }
}

seq_t a(idx_t n)
{
    /* n is 1‑based and guaranteed ≥ 1. */
    static seq_t *ps = NULL;   /* partition numbers p[0…] */
    static size_t cap = 0;     /* array capacity */
    static size_t rows = 0;    /* number of rows computed so far */
    static idx_t cumulative = 0;  /* total partitions seen so far */

    if (ps == NULL) {
        /* initial capacity; ps[0] is needed for recurrence */
        cap = 2;
        ps = (seq_t *)malloc(cap * sizeof(seq_t));
        ps[0] = 1;                /* p(0) = 1 for recurrence */
    }

    /* extend the table until we have enough partitions to cover index n */
    while (cumulative < n) {
        size_t r = rows + 1;          /* next row number */

        if (r >= cap) {
            size_t new_cap = cap * 2;
            while (new_cap <= r)
                new_cap *= 2;
            ps = (seq_t *)realloc(ps, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* compute p(r) using Euler's pentagonal recurrence */
        __int128 sum = 0;
        for (int k = 1; ; ++k) {
            int g1 = k * (3 * k - 1) / 2;
            if (g1 > (int)r)
                break;
            __int128 term = (__int128)ps[r - g1];
            if (k & 1)   /* odd k */
                sum += term;
            else
                sum -= term;

            int g2 = k * (3 * k + 1) / 2;
            if (g2 <= (int)r) {
                term = (__int128)ps[r - g2];
                if (k & 1)
                    sum += term;
                else
                    sum -= term;
            }
        }
        seq_t p_r = (seq_t)sum;
        ps[r] = p_r;

        cumulative += (idx_t)p_r;
        rows = r;
    }

    /* Determine which partition within the identified row corresponds to n */
    idx_t prev_cumulative = cumulative - (idx_t)ps[rows];
    idx_t pos = n - prev_cumulative;  /* 1‑based position within the row */

    /* Generate the pos‑th partition of 'rows' and compute its alternating sum */
    int parts_arr[512];                /* depths are very small (<500) */
    idx_t counter = 0;
    int found_flag = 0;
    seq_t answer = 0;

    gen_partitions((int)rows, (int)rows, 0, pos, &counter, &answer, &found_flag, parts_arr);

    return answer;
}
