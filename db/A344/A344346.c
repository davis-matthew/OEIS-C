/*
 * A344346  Numbers k which have an odd number of trailing zeros in their
 *          binary reflected Gray code A014550(k).
 * https://oeis.org/A344346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A081706(n) + 1
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A081706 lists the integers k such that the Gray code of k
    has an odd number of trailing zeros.  The sequence here is that
    list shifted by +1, so a(n) is the (n‑th) element of A081706
    plus one.  We generate the list on demand and memoise it.  */

#include <stdlib.h>

/* static state for memoisation  */
static seq_t *seq   = NULL;   /* array of found elements           */
static size_t seq_len = 0;    /* number of elements currently stored */
static size_t seq_cap = 0;    /* current allocated capacity        */
static idx_t  last_k   = 0;   /* largest k examined so far        */

/* append a new element to the memoised array, growing it if necessary */
static void append(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap << 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unable to allocate memory */
        seq   = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* extend memoised sequence until it contains at least `need` elements   */
static void ensure_upto(idx_t need)
{
    while (seq_len < (size_t)need) {
        ++last_k;                                   /* next k to test   */
        seq_t gray = ((seq_t)last_k) ^ (((seq_t)last_k) >> 1);
        int z = 0;                                   /* trailing zero count */
        seq_t g = gray;
        while ((g & 1) == 0) {                       /* count zeros         */
            ++z;
            g >>= 1;
        }
        if (z & 1) {                                 /* odd number of zeros */
            append((seq_t)last_k);                   /* store k             */
        }
    }
}

/* public function required by the interface */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                                   /* undefined below offset */
    ensure_upto(n);
    return seq[n - 1];
}
