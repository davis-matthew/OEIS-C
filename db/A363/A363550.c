/*
 * A363550  Number of partitions of [n] having exactly one parity change
 *          within the partition.
 * https://oeis.org/A363550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=0; for n≥2: if n is odd a(n)=Bell((n-1)/2) if n is
 *            even a(n)=Bell(n/2)+Bell(n/2-1) where Bell(m) is the m‑th Bell
 *            number (Bell(0)=1)
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 31, u64 n <= 51, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of Bell numbers: bell[i] = Bell(i) for i = 0..bell_max */
static seq_t *bell   = NULL;
static size_t bell_size = 0;
static idx_t  bell_max  = 0;

/* Ensure the Bell cache contains values up to max_k (inclusive). */
static void compute_bell_up_to(idx_t max_k)
{
    if (max_k <= bell_max) return;

    /* Resize the array if needed */
    size_t new_size = bell_size ? bell_size : 16;
    while (new_size <= (size_t)max_k) new_size <<= 1;
    seq_t *tmp = (seq_t *)realloc(bell, new_size * sizeof(seq_t));
    bell   = tmp;
    bell_size = new_size;

    /* Initialise the first Bell number */
    if (bell_max == 0 && bell_size > 0) {
        bell[0] = (seq_t)1;
    }

    /* Compute Bell numbers from bell_max+1 up to max_k using
       Bell(n+1) = Σ_{k=0}^{n} C(n,k) * Bell(k)   (n = current_index-1). */
    for (idx_t n = bell_max + 1; n <= max_k; ++n) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* C(n-1,0) */
        idx_t nm1 = n - 1;
        for (idx_t k = 0; k <= nm1; ++k) {
            sum += bin * bell[k];
            if (k < nm1) {
                bin = bin * (seq_t)(nm1 - k);   /* multiply by (n-1-k) */
                bin = bin / (seq_t)(k + 1);     /* divide by (k+1); exact division */
            }
        }
        bell[n] = sum;
    }
    bell_max = max_k;
}

seq_t a(idx_t n)
{
    /* Handle offset values explicitly */
    if (n == 0 || n == 1) return (seq_t)0;

    /* Determine how many Bell numbers are needed */
    idx_t max_bell = n / 2;            /* floor(n/2) is enough for both cases */

    compute_bell_up_to(max_bell);

    if ((n & 1) == 1) {                 /* n odd */
        return bell[(n - 1) / 2];
    } else {                            /* n even (n≥2) */
        return bell[n / 2] + bell[n / 2 - 1];
    }
}
