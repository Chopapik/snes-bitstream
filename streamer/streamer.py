from pynput.keyboard import Controller, Key
import time

BIT_MAP_P1 = {
    0: 'v',        # bit 0 -> SNES A
    1: 'c',        # bit 1 -> SNES B
    2: 'd',        # bit 2 -> SNES X
    3: 'x',        # bit 3 -> SNES Y
    4: 'a',        # bit 4 -> SNES L
    5: 's',        # bit 5 -> SNES R
    6: Key.space,  # bit 6 -> SNES Start
    7: Key.enter,  # bit 7 -> SNES Select
}

KEY_STROBE = Key.up  # SNES Up = strobe signal

class Streamer:
    def __init__(self):
        self.kb = Controller()

    def release_all(self):
        """Release all used keys."""
        keys = [Key.up, 'v', 'c', 'd', 'x', 'a', 's', Key.space, Key.enter]
        for key in keys:
            try:
                self.kb.release(key)
            except Exception:
                pass

    def _send_parallel_byte(self, byte_val,
                           hold_time=0.050,
                           inter_byte=0.040,
                           log=True):
        byte_val &= 0xFF

        # 0. Clear previous state
        self.release_all()
        time.sleep(0.010)

        pressed_keys = []

        # 1. Set states of the bits
        for bit, key in BIT_MAP_P1.items():
            if (byte_val >> bit) & 1:
                self.kb.press(key)
                pressed_keys.append(key)
        # 2. Strobe with UP key
        self.kb.press(KEY_STROBE)

        # 3. Hold the strobe for a moment
        time.sleep(hold_time)

        # 4. Release everything
        self.kb.release(KEY_STROBE)
        for key in pressed_keys:
            self.kb.release(key)

        if log:
            ch = chr(byte_val) if 32 <= byte_val <= 126 else '.'
            print(f"[0x{byte_val:02X}] '{ch}'", flush=True)

        # 5. Pause between bytes
        time.sleep(inter_byte)

    def send_message(self, message):
        print(f"\n[streamer] Starting transmission: {message}")
        print("-" * 40)

        if isinstance(message, str):
            data = message.encode("ascii", errors="replace")
        else:
            data = bytes(message)

        for b in data:
            self._send_parallel_byte(b)

        print("-" * 40)
        print("[streamer] Done.")
        """
        Test of individual bits:
        0x01, 0x02, 0x04, ...
        This allows you to check the mapping in the emulator.
        """
        print("\n[streamer] Bit mapping test")
        print("-" * 40)
        for b in [0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80]:
            self._send_parallel_byte(b)
        print("-" * 40)
        print("[streamer] Test finished.")

if __name__ == "__main__":
    streamer = Streamer()

    print("!!! WARNING !!!")
    print("You have 3 seconds to click in the SNES9x emulator window.")
    print("Joypad 1 (Snes9x): Up=Up, A=V, B=C, X=D, Y=X, L=A, R=S, Start=Space, Select=Return")
    print("-" * 40)

    time.sleep(3)

    text_to_send = "Hello from Python Parallel BBS!"
    streamer.send_message(text_to_send)