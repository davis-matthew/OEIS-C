/*
 * A269868  Lexicographically earliest sequence of distinct positive integers
 *          such that all values of (a(n) XOR n) are distinct.
 * https://oeis.org/A269868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 168, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = lexicographically earliest sequence of distinct positive
 * integers such that all values of (a(n) XOR n) are distinct.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *seq_arr   = NULL;   /* a(1)…a(max_n)                  */
static size_t  seq_cap   = 0;      /* capacity of seq_arr             */
static uint8_t *used_a   = NULL;  /* bitset of used a-values        */
static size_t  used_a_cap = 0;    /* capacity of used_a             */
static uint8_t *used_xor = NULL;  /* bitset of used XOR-values      */
static size_t  used_xor_cap = 0;  /* capacity of used_xor           */
static idx_t   max_n = 0;          /* number of terms already computed */

/* grow a byte array to accommodate `need` elements (0‑based index) */
static void ensure_cap(uint8_t **arr, size_t *cap, size_t need)
{
    if (*cap > need) return;
    size_t newcap = (*cap > 0 ? *cap << 1 : 128);
    if (newcap <= need) newcap = need + 128;
    uint8_t *tmp = (uint8_t *)realloc(*arr, newcap);
    if (!tmp) abort();
    memset(tmp + *cap, 0, newcap - *cap);
    *arr = tmp;
    *cap = newcap;
}

/* ensure seq_arr can hold `need` terms */
static void ensure_seq_cap(idx_t need)
{
    if (seq_cap >= (size_t)need) return;
    size_t newcap = (seq_cap > 0 ? seq_cap << 1 : 128);
    if (newcap <= (size_t)need) newcap = (size_t)need + 128;
    seq_t *tmp = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
    if (!tmp) abort();
    seq_arr = tmp;
    seq_cap = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1 */
    if (n <= max_n) return seq_arr[n-1];      /* cached value */

    /* compute terms up to n */
    for (idx_t cur = max_n + 1; cur <= n; ++cur)
    {
        for (idx_t cand = 1; ; ++cand)
        {
            /* ensure we can test candidate `cand` */
            if ((size_t)cand >= used_a_cap)
                ensure_cap(&used_a, &used_a_cap, (size_t)cand + 1);

            if (!used_a[(size_t)cand])          /* unused a-value? */
            {
                seq_t xor_val = (seq_t)cand ^ (seq_t)cur;
                size_t x = (size_t)xor_val;     /* must fit in size_t */

                /* ensure space for xor value */
                if (x >= used_xor_cap)
                    ensure_cap(&used_xor, &used_xor_cap, x + 1);

                if (!used_xor[x])                /* xor value unused? */
                {
                    /* accept this candidate */
                    seq_t val = (seq_t)cand;
                    ensure_seq_cap(cur);
                    seq_arr[cur-1] = val;

                    used_a[(size_t)cand] = 1;
                    used_xor[x] = 1;
                    break;
                }
            }
        }
    }

    max_n = n;
    return seq_arr[n-1];
}
