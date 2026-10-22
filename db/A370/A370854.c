/*
 * A370854  Numbers m such that c(0) <= c(1) < c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers m such that c(0) <= c(1) < c(2) where * c(k) is the
 *            count of digit k in the ternary representation of m. *
 *            Implementation generates terms on demand and caches them
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif

static seq_t next_start = 0;   /* next integer to test */

/* ---------- C version ---------- */
#ifndef __cplusplus
static seq_t *seq = NULL;      /* cached sequence */
static size_t seq_cap = 0;     /* capacity of 'seq' */
static size_t seq_len = 0;     /* number of terms stored */
#define CUR_LEN seq_len
#else
/* ---------- C++ version ---------- */
static std::vector<seq_t> seqVec;
#define CUR_LEN seqVec.size()
#endif

#ifndef __cplusplus
static void grow_seq(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1); /* abort on allocation failure */
    seq = tmp;
    seq_cap = new_cap;
}
#endif

static void generate_until(size_t target)
{
    while (CUR_LEN < target)
    {
        seq_t m = next_start;
        unsigned c0 = 0, c1 = 0, c2 = 0;
        seq_t t = m;

        /* count digits in ternary representation of m */
        if (t == 0)
            c0 = 1;
        else
        {
            while (t)
            {
                unsigned d = (unsigned)(t % 3);
                if (d == 0)
                    ++c0;
                else if (d == 1)
                    ++c1;
                else
                    ++c2;                /* d == 2 */
                t /= 3;
            }
        }

        if (c0 <= c1 && c1 < c2)
        {
#ifndef __cplusplus
            if (seq_len >= seq_cap)
                grow_seq(seq_cap == 0 ? 64 : seq_cap * 2);
            seq[seq_len++] = m;
#else
            seqVec.push_back(m);
#endif
        }
        ++next_start;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* defined only for n >= 1 */
    generate_until((size_t)n);
#ifndef __cplusplus
    return seq[(size_t)(n - 1)];
#else
    return seqVec[(size_t)(n - 1)];
#endif
}
