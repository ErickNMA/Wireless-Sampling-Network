import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, get_window
from scipy.io import savemat
import scienceplots

plt.style.use([
    'grid',
    'notebook',
    'retro'
])

data = np.load('data_lucio_motor.npy')

accelerations = data[:, :, :3]
gyros         = data[:, :, 3:]

g_scale = 8192

x_accel = accelerations[:,:,0].flatten() / g_scale
y_accel = accelerations[:,:,1].flatten() / g_scale
z_accel = accelerations[:,:,2].flatten() / g_scale

savemat('data.mat', {
    'x_accel': x_accel,
    'y_accel': y_accel,
    'z_accel': z_accel,
})

pitch = gyros[:,:,0].flatten()
roll  = gyros[:,:,1].flatten()
yaw   = gyros[:,:,2].flatten()

window_name = 'hamming'

def fft(signal, sampling_time):
    N = signal.shape[0]
    
    window = get_window(window_name, N)
    windowed_signal = signal * window
    
    fft = np.fft.fft(windowed_signal)

    f = np.fft.fftfreq(N, sampling_time)

    K = N // 2

    freqs = f[:K]
    amplitudes = np.abs(fft)[:K]

    phase = np.rad2deg(np.angle(fft)[:K])

    return freqs, amplitudes, phase

fft_x = fft(x_accel, 1e-3)
fft_y = fft(y_accel, 1e-3)
fft_z = fft(z_accel, 1e-3)

tempo = np.arange(0, (len(x_accel)*1e-3), 1e-3)

plt.figure()

# plt.loglog()
# plt.plot(fft_z[0], fft_z[1], label='z accel')
# plt.plot(fft_x[0], fft_x[1], label='x accel')
# plt.plot(fft_y[0], fft_y[1], label='y accel')

# plt.xlabel('Frequency (Hz)')
# plt.ylabel('Amplitude (g)')
# plt.legend()

# fft_pitch = fft(pitch, 1e-3)
# fft_roll  = fft(roll, 1e-3)
# fft_yaw   = fft(yaw, 1e-3)

plt.plot(tempo, x_accel, label='x accel')
plt.plot(tempo, y_accel, label='y accel')
plt.plot(tempo, z_accel, label='z accel')

plt.xlabel('Tempo (s)')
plt.ylabel('Amplitude (g)')
plt.legend()

#plt.show()

plt.savefig('tempo_lucio_motor.eps', dpi=600, transparent=True, bbox_inches='tight')